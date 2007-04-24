#include "StdAfx.h"
#include "DiskID.h"
# include "conector.h"

#include "seguridad/port32.h"
#include "seguridad/winio.h"


#pragma pack(1)


#define  IDENTIFY_BUFFER_SIZE  512


//  IOCTL commands
#define  DFP_GET_VERSION          0x00074080
#define  DFP_SEND_DRIVE_COMMAND   0x0007c084
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088

#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition

//  GETVERSIONOUTPARAMS contains the data returned from the 
//  Get Driver Version function.
typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

//  Bits returned in the fCapabilities member of GETVERSIONOUTPARAMS 
#define  CAP_IDE_ID_FUNCTION             1  // ATA ID command supported
#define  CAP_IDE_ATAPI_ID                2  // ATAPI ID command supported
#define  CAP_IDE_EXECUTE_SMART_FUNCTION  4  // SMART commannds supported


//  Valid values for the bCommandReg member of IDEREGS.
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.

// The following struct defines the interesting part of the IDENTIFY
// buffer:
typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength;
	UCHAR Signature[8];
	ULONG Timeout;
	ULONG ControlCode;
	ULONG ReturnCode;
	ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

//  Max number of drives assuming primary/secondary, master/slave topology
#define  MAX_IDE_DRIVES  16

int CDiskID::ReadPhysicalDriveInNTWithAdminRights() {
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		char driveName [256];

		sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);

		//  Windows NT, Windows 2000, must have admin rights
		hPhysicalDriveIOCTL = CreateFile (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE , NULL,
			OPEN_EXISTING, 0, NULL);
		// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
		//            drive, GetLastError ());

		if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			GETVERSIONOUTPARAMS VersionParams;
			DWORD               cbBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL
			memset ((void*) &VersionParams, 0, sizeof(VersionParams));

			if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL, 
				0,
				&VersionParams,
				sizeof(VersionParams),
				&cbBytesReturned, NULL) )
			{         
				// printf ("DFP_GET_VERSION failed for drive %d\n", i);
				// continue;
			}

			// If there is a IDE device at number "i" issue commands
			// to the device
			if (VersionParams.bIDEDeviceMap > 0)
			{
				BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
				SENDCMDINPARAMS  scip;
				//SENDCMDOUTPARAMS OutCmd;

				// Now, get the ID sector for all IDE devices in the system.
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
				// otherwise use the IDE_ATA_IDENTIFY command
				bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

				memset (&scip, 0, sizeof(scip));
				memset (IdOutCmd, 0, sizeof(IdOutCmd));

				if ( DoIDENTIFY (hPhysicalDriveIOCTL, 
					&scip, 
					(PSENDCMDOUTPARAMS)&IdOutCmd, 
					(BYTE) bIDCmd,
					(BYTE) drive,
					&cbBytesReturned))
				{
					DWORD diskdata [256];
					int ijk = 0;
					USHORT *pIdSector = (USHORT *)
						((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;

					for (ijk = 0; ijk < 256; ijk++)
						diskdata [ijk] = pIdSector [ijk];

					PrintIdeInfo (drive, diskdata);

					done = TRUE;
				}
			}

			CloseHandle (hPhysicalDriveIOCTL);
		}
	}
	return done;
}


#pragma pack(4)

//typedef enum _STORAGE_QUERY_TYPE {
//	PropertyStandardQuery = 0,          // Retrieves the descriptor
//	PropertyExistsQuery,                // Used to test whether the descriptor is supported
//	PropertyMaskQuery,                  // Used to retrieve a mask of writeable fields in the descriptor
//	PropertyQueryMaxDefined     // use to validate the value
//} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

//
// define some initial property id's
//

//typedef enum _STORAGE_PROPERTY_ID {
//	StorageDeviceProperty = 0,
//	StorageAdapterProperty
//} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

//
// Query structure - additional parameters for specific queries can follow
// the header
//

//typedef struct _STORAGE_PROPERTY_QUERY {
//
//	//
//	// ID of the property being retrieved
//	//
//
//	STORAGE_PROPERTY_ID PropertyId;
//
//	//
//	// Flags indicating the type of query being performed
//	//
//
//	STORAGE_QUERY_TYPE QueryType;
//
//	//
//	// Space for additional parameters if necessary
//	//
//
//	UCHAR AdditionalParameters[1];
//
//} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;


#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)


//
// Device property descriptor - this is really just a rehash of the inquiry
// data retrieved from a scsi device
//
// This may only be retrieved from a target device.  Sending this to the bus
// will result in an error
//

#pragma pack(4)

//typedef struct _STORAGE_DEVICE_DESCRIPTOR {
//
//	//
//	// Sizeof(STORAGE_DEVICE_DESCRIPTOR)
//	//
//
//	ULONG Version;
//
//	//
//	// Total size of the descriptor, including the space for additional
//	// data and id strings
//	//
//
//	ULONG Size;
//
//	//
//	// The SCSI-2 device type
//	//
//
//	UCHAR DeviceType;
//
//	//
//	// The SCSI-2 device type modifier (if any) - this may be zero
//	//
//
//	UCHAR DeviceTypeModifier;
//
//	//
//	// Flag indicating whether the device's media (if any) is removable.  This
//	// field should be ignored for media-less devices
//	//
//
//	BOOLEAN RemovableMedia;
//
//	//
//	// Flag indicating whether the device can support mulitple outstanding
//	// commands.  The actual synchronization in this case is the responsibility
//	// of the port driver.
//	//
//
//	BOOLEAN CommandQueueing;
//
//	//
//	// Byte offset to the zero-terminated ascii string containing the device's
//	// vendor id string.  For devices with no such ID this will be zero
//	//
//
//	ULONG VendorIdOffset;
//
//	//
//	// Byte offset to the zero-terminated ascii string containing the device's
//	// product id string.  For devices with no such ID this will be zero
//	//
//
//	ULONG ProductIdOffset;
//
//	//
//	// Byte offset to the zero-terminated ascii string containing the device's
//	// product revision string.  For devices with no such string this will be
//	// zero
//	//
//
//	ULONG ProductRevisionOffset;
//
//	//
//	// Byte offset to the zero-terminated ascii string containing the device's
//	// serial number.  For devices with no serial number this will be zero
//	//
//
//	ULONG SerialNumberOffset;
//
//	//
//	// Contains the bus type (as defined above) of the device.  It should be
//	// used to interpret the raw device properties at the end of this structure
//	// (if any)
//	//
//
//	STORAGE_BUS_TYPE BusType;
//
//	//
//	// The number of bytes of bus-specific data which have been appended to
//	// this descriptor
//	//
//
//	ULONG RawPropertiesLength;
//
//	//
//	// Place holder for the first byte of the bus specific property data
//	//
//
//	UCHAR RawDeviceProperties[1];
//
//} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

char *CDiskID::flipAndCodeBytes (char * str)
{
	static char flipped [1000];
	int i = 0;
	int j = 0;
	int k = 0;
	int num = strlen (str);

	strcpy (flipped, "");
	for (i = 0; i < num; i += 4)
	{
		for (j = 1; j >= 0; j--)
		{
			int sum = 0;
			for (k = 0; k < 2; k++)
			{
				sum *= 16;
				switch (str [i + j * 2 + k])
				{
				case '0': sum += 0; break;
				case '1': sum += 1; break;
				case '2': sum += 2; break;
				case '3': sum += 3; break;
				case '4': sum += 4; break;
				case '5': sum += 5; break;
				case '6': sum += 6; break;
				case '7': sum += 7; break;
				case '8': sum += 8; break;
				case '9': sum += 9; break;
				case 'a': sum += 10; break;
				case 'b': sum += 11; break;
				case 'c': sum += 12; break;
				case 'd': sum += 13; break;
				case 'e': sum += 14; break;
				case 'f': sum += 15; break;
				case 'A': sum += 10; break;
				case 'B': sum += 11; break;
				case 'C': sum += 12; break;
				case 'D': sum += 13; break;
				case 'E': sum += 14; break;
				case 'F': sum += 15; break;
				}
			}
			if (sum > 0) 
			{
				char sub [2];
				sub [0] = (char) sum;
				sub [1] = 0;
				strcat (flipped, sub);
			}
		}
	}

	return flipped;
}

typedef struct _MEDIA_SERAL_NUMBER_DATA {
	ULONG  SerialNumberLength; 
	ULONG  Result;
	ULONG  Reserved[2];
	UCHAR  SerialNumberData[1];
} MEDIA_SERIAL_NUMBER_DATA, *PMEDIA_SERIAL_NUMBER_DATA;

int CDiskID::ReadPhysicalDriveInNTWithZeroRights (void)
{
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		char driveName [256];

		sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);

		//  Windows NT, Windows 2000, Windows XP - admin rights not required
		hPhysicalDriveIOCTL = CreateFile (driveName, 0,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
		//            drive, GetLastError ());

		if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			STORAGE_PROPERTY_QUERY query;
			DWORD cbBytesReturned = 0;
			char buffer [10000];

			memset ((void *) & query, 0, sizeof (query));
			query.PropertyId = StorageDeviceProperty;
			query.QueryType = PropertyStandardQuery;

			memset (buffer, 0, sizeof (buffer));

			if ( DeviceIoControl (hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY,
				& query,
				sizeof (query),
				& buffer,
				sizeof (buffer),
				& cbBytesReturned, NULL) )
			{         
				STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *) & buffer;
				char serialNumber [1000];
				char modelNumber [1000];

				strcpy (serialNumber, flipAndCodeBytes ( & buffer [descrip -> SerialNumberOffset]));
				strcpy (modelNumber, & buffer [descrip -> ProductIdOffset]);
				AddDiscoDuro(serialNumber);
			}
			else
		 {
			 DWORD err = GetLastError ();
		 }

			memset (buffer, 0, sizeof (buffer));

			if ( DeviceIoControl (hPhysicalDriveIOCTL, IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER,
				NULL,
				0,
				& buffer,
				sizeof (buffer),
				& cbBytesReturned, NULL) )
			{         
				MEDIA_SERIAL_NUMBER_DATA * mediaSerialNumber = 
					(MEDIA_SERIAL_NUMBER_DATA *) & buffer;
				char serialNumber [1000];
				// char modelNumber [1000];

				strcpy (serialNumber, (char *) mediaSerialNumber -> SerialNumberData);
				AddDiscoDuro(serialNumber);
		 }
			else
		 {
			 DWORD err = GetLastError ();
		 }

			CloseHandle (hPhysicalDriveIOCTL);
		}
	}

	return done;
}

BOOL CDiskID::DoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
				 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
				 PDWORD lpcbBytesReturned)
{
	// Set up data structures for IDENTIFY command.
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP -> irDriveRegs.bFeaturesReg = 0;
	pSCIP -> irDriveRegs.bSectorCountReg = 1;
	//pSCIP -> irDriveRegs.bSectorNumberReg = 1;
	pSCIP -> irDriveRegs.bCylLowReg = 0;
	pSCIP -> irDriveRegs.bCylHighReg = 0;

	// Compute the drive number.
	pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

	// The command can either be IDE identify or ATAPI identify.
	pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
	pSCIP -> bDriveNumber = bDriveNum;
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

	return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		(LPVOID) pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID) pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL) );
}


// (* Output Bbuffer for the VxD (rt_IdeDinfo record) *)
typedef struct _rt_IdeDInfo_
{
	BYTE IDEExists[4];
	BYTE DiskExists[8];
	WORD DisksRawInfo[8*256];
} rt_IdeDInfo, *pt_IdeDInfo;


// (* IdeDinfo "data fields" *)
typedef struct _rt_DiskInfo_
{
	BOOL DiskExists;
	BOOL ATAdevice;
	BOOL RemovableDevice;
	WORD TotLogCyl;
	WORD TotLogHeads;
	WORD TotLogSPT;
	char SerialNumber[20];
	char FirmwareRevision[8];
	char ModelNumber[40];
	WORD CurLogCyl;
	WORD CurLogHeads;
	WORD CurLogSPT;
} rt_DiskInfo;


#define  m_cVxDFunctionIdesDInfo  1

int CDiskID::ReadDrivePortsInWin9X (void)
{
	int done = FALSE;
	unsigned long int i = 0;

	HANDLE VxDHandle = 0;
	pt_IdeDInfo pOutBufVxD = 0;
	DWORD lpBytesReturned = 0;

	//  set the thread priority high so that we get exclusive access to the disk
	BOOL status =
		// SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		SetPriorityClass (GetCurrentProcess (), REALTIME_PRIORITY_CLASS);

	// 1. Make an output buffer for the VxD
	rt_IdeDInfo info;
	pOutBufVxD = &info;

	// *****************
	// KLUDGE WARNING!!!
	// HAVE to zero out the buffer space for the IDE information!
	// If this is NOT done then garbage could be in the memory
	// locations indicating if a disk exists or not.
	ZeroMemory (&info, sizeof(info));

	// 1. Try to load the VxD
	//  must use the short file name path to open a VXD file
	//char StartupDirectory [2048];
	//char shortFileNamePath [2048];
	//char *p = NULL;
	//char vxd [2048];
	//  get the directory that the exe was started from
	//GetModuleFileName (hInst, (LPSTR) StartupDirectory, sizeof (StartupDirectory));
	//  cut the exe name from string
	//p = &(StartupDirectory [strlen (StartupDirectory) - 1]);
	//while (p >= StartupDirectory && *p && '\\' != *p) p--;
	//*p = '\0';   
	//GetShortPathName (StartupDirectory, shortFileNamePath, 2048);
	//sprintf (vxd, "\\\\.\\%s\\IDE21201.VXD", shortFileNamePath);
	//VxDHandle = CreateFile (vxd, 0, 0, 0,
	//               0, FILE_FLAG_DELETE_ON_CLOSE, 0);   
	VxDHandle = CreateFile ("\\\\.\\IDE21201.VXD", 0, 0, 0,
		0, FILE_FLAG_DELETE_ON_CLOSE, 0);

	if (VxDHandle != INVALID_HANDLE_VALUE)
	{
		// 2. Run VxD function
		DeviceIoControl (VxDHandle, m_cVxDFunctionIdesDInfo,
			0, 0, pOutBufVxD, sizeof(pt_IdeDInfo), &lpBytesReturned, 0);

		// 3. Unload VxD
		CloseHandle (VxDHandle);
	}
	else
		MessageBox (NULL, "ERROR: Could not open IDE21201.VXD file", 
		CApplicacion::GetAppName(), MB_ICONSTOP);

	// 4. Translate and store data
	for (i=0; i<8; i++)
	{
		if((pOutBufVxD->DiskExists[i]) && (pOutBufVxD->IDEExists[i/2]))
		{
			DWORD diskinfo [256];
			for (int j = 0; j < 256; j++) 
				diskinfo [j] = pOutBufVxD -> DisksRawInfo [i * 256 + j];

			// process the information for this buffer
			PrintIdeInfo (i, diskinfo);
			done = TRUE;
		}
	}

	//  reset the thread priority back to normal
	// SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_NORMAL);
	SetPriorityClass (GetCurrentProcess (), NORMAL_PRIORITY_CLASS);

	return done;
}

#define  SENDIDLENGTH  sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE

int CDiskID::ReadIdeDriveAsScsiDriveInNT (void)
{
	int done = FALSE;
	int controller = 0;

	for (controller = 0; controller < 16; controller++)
	{
		HANDLE hScsiDriveIOCTL = 0;
		char   driveName [256];

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		sprintf (driveName, "\\\\.\\Scsi%d:", controller);

		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFile (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		// if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
		//            controller, GetLastError ());

		if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			int drive = 0;

			for (drive = 0; drive < 2; drive++)
			{
				char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
				SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
				SENDCMDINPARAMS *pin =
					(SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
				DWORD dummy;

				memset (buffer, 0, sizeof (buffer));
				p -> HeaderLength = sizeof (SRB_IO_CONTROL);
				p -> Timeout = 10000;
				p -> Length = SENDIDLENGTH;
				p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy ((char *) p -> Signature, "SCSIDISK", 8);

				pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin -> bDriveNumber = drive;

				if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
					buffer,
					sizeof (SRB_IO_CONTROL) +
					sizeof (SENDCMDINPARAMS) - 1,
					buffer,
					sizeof (SRB_IO_CONTROL) + SENDIDLENGTH,
					&dummy, NULL))
				{
					SENDCMDOUTPARAMS *pOut =
						(SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
					IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);
					if (pId -> sModelNumber [0])
					{
						DWORD diskdata [256];
						int ijk = 0;
						USHORT *pIdSector = (USHORT *) pId;

						for (ijk = 0; ijk < 256; ijk++)
							diskdata [ijk] = pIdSector [ijk];

						PrintIdeInfo (controller * 2 + drive, diskdata);

						done = TRUE;
					}
				}
			}
			CloseHandle (hScsiDriveIOCTL);
		}
	}

	return done;
}

void CDiskID::PrintIdeInfo (int drive, DWORD diskdata [256])
{
	char HardDriveSerialNumber[1024];
	//  copy the hard drive serial number to the buffer
	strcpy (HardDriveSerialNumber, ConvertToString (diskdata, 10, 19));
	// esta linea es la mia
	AddDiscoDuro(HardDriveSerialNumber);

	/*__int64 sectors = 0;
	__int64 bytes = 0;
	sprintf (string1, "Drive%dModelNumber", drive);
	WriteConstantString (string1, ConvertToString (diskdata, 27, 46));
	sprintf (string1, "Drive%dSerialNumber", drive);
	WriteConstantString (string1, ConvertToString (diskdata, 10, 19));
	sprintf (string1, "Drive%dControllerRevisionNumber", drive);
	WriteConstantString (string1, ConvertToString (diskdata, 23, 26));
	sprintf (string1, "Drive%dControllerBufferSize", drive);
	sprintf (string2, "%d", diskdata [21] * 512);
	WriteConstantString (string1, string2);*/

	/*sprintf (string1, "Drive%dType", drive);
	if (diskdata [0] & 0x0080)
		WriteConstantString (string1, "Removable");
	else if (diskdata [0] & 0x0040)
		WriteConstantString (string1, "Fixed");
	else
		WriteConstantString (string1, "Unknown");*/
}

char *CDiskID::ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex)
{
	static char string [1024];
	int index = 0;
	int position = 0;

	//  each integer has two characters stored in it backwards
	for (index = firstIndex; index <= lastIndex; index++)
	{
		//  get high byte for 1st character
		string [position] = (char) (diskdata [index] / 256);
		position++;

		//  get low byte for 2nd character
		string [position] = (char) (diskdata [index] % 256);
		position++;
	}

	//  end the string 
	string [position] = '\0';

	//  cut off the trailing blanks
	for (index = position - 1; index > 0 && ' ' == string [index]; index--)
		string [index] = '\0';

	return string;
}

long CDiskID::getHardDriveComputerID ()
{
	int done = FALSE;
	// char string [1024];
	__int64 id = 0;
	OSVERSIONINFO version;

	memset (&version, 0, sizeof (version));
	version.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	GetVersionEx (&version);
	if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		//  this works under WinNT4 or Win2K if you have admin rights
		done = ReadPhysicalDriveInNTWithAdminRights ();

		//  this should work in WinNT or Win2K if previous did not work
		//  this is kind of a backdoor via the SCSI mini port driver into
		//     the IDE drives
		// if ( ! done) 
		done = ReadIdeDriveAsScsiDriveInNT ();

		//  this works under WinNT4 or Win2K or WinXP if you have any rights
		//if ( ! done)
		done = ReadPhysicalDriveInNTWithZeroRights ();

	}
	else
	{
		//  this works under Win9X and calls a VXD
		//  try this up to 10 times to get a hard drive serial number
		done = ReadDrivePortsInWin9X ();
	}
	return done;
}

CDiskID::CDiskID(void)
{
	getHardDriveComputerID();
}

CDiskID::~CDiskID(void)
{
}

int CDiskID::GetDiscosDuros() {

	if(m_szIDDiscosDuros.IsEmpty())
		m_szIDDiscosDuros.Add("esteesmiserialgenerico");
	return m_szIDDiscosDuros.GetCount();
}

unsigned int CDiskID::GetSerialNumber(int index/*=-1*/) {

	if(index==-1) {
		for(int i=m_szIDDiscosDuros.GetCount()-1; i>=0; i--) {
			if(m_szIDDiscosDuros[i].Find(' ')==-1) {
				index= i;
				break;
			}
		}
		if(index==-1)
			index=GetDiscosDuros()-1;
	}
	
	unsigned __int64 id = 0;
	char HardDriveSerialNumber[1024];
	strcpy (HardDriveSerialNumber, m_szIDDiscosDuros.GetAt(index));
	
	char *p = HardDriveSerialNumber;

	//WriteConstantString ("HardDriveSerialNumber", HardDriveSerialNumber);

	//  ignore first 5 characters from western digital hard drives if
	//  the first four characters are WD-W
	if ( ! strncmp (HardDriveSerialNumber, "WD-W", 4)) p += 5;
	for ( ; p && *p; p++)
	{
		if ('-' == *p) continue;
		id *= 10;
		switch (*p)
		{
		case '0': id += 0; break;
		case '1': id += 1; break;
		case '2': id += 2; break;
		case '3': id += 3; break;
		case '4': id += 4; break;
		case '5': id += 5; break;
		case '6': id += 6; break;
		case '7': id += 7; break;
		case '8': id += 8; break;
		case '9': id += 9; break;
		case 'a': case 'A': id += 10; break;
		case 'b': case 'B': id += 11; break;
		case 'c': case 'C': id += 12; break;
		case 'd': case 'D': id += 13; break;
		case 'e': case 'E': id += 14; break;
		case 'f': case 'F': id += 15; break;
		case 'g': case 'G': id += 16; break;
		case 'h': case 'H': id += 17; break;
		case 'i': case 'I': id += 18; break;
		case 'j': case 'J': id += 19; break;
		case 'k': case 'K': id += 20; break;
		case 'l': case 'L': id += 21; break;
		case 'm': case 'M': id += 22; break;
		case 'n': case 'N': id += 23; break;
		case 'o': case 'O': id += 24; break;
		case 'p': case 'P': id += 25; break;
		case 'q': case 'Q': id += 26; break;
		case 'r': case 'R': id += 27; break;
		case 's': case 'S': id += 28; break;
		case 't': case 'T': id += 29; break;
		case 'u': case 'U': id += 30; break;
		case 'v': case 'V': id += 31; break;
		case 'w': case 'W': id += 32; break;
		case 'x': case 'X': id += 33; break;
		case 'y': case 'Y': id += 34; break;
		case 'z': case 'Z': id += 35; break;
		}                            
	}
	//   make sure no bigger than 16^7
	if (id > 268435455) 
		id %= 268435456;
	return (unsigned int) id;
}

void CDiskID::AddDiscoDuro(CString cadena) {

	if(cadena.IsEmpty())
		return;
	// solo se guardan los seriales con espacio por compatibilidad con versiones anteriores
	for(int i=0; i<m_szIDDiscosDuros.GetCount(); i++) {
		if(m_szIDDiscosDuros[i]==cadena)
			return;
	}
	m_szIDDiscosDuros.Add(cadena);
	if(cadena.Find(' ')!=-1) {
		cadena.Trim();
		AddDiscoDuro(cadena);
	}
}

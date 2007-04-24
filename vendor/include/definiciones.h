#pragma once
# include "errorreport/functionstacktrace.h"

// procesador de mensajes

// mensaje del spliter cuando se redimenciona.
# define SPN_SIZED WM_USER + 1001

// mensaje del combobox cuando se actualiza
# define WM_REALUPDATECOMBO 	WM_USER+1002
# define WM_THREADSTAT 		WM_USER+1003 // mensaje del hilo de estadisticas

// mensaje de las barras de herramientas
# define	WM_JDOCK	WM_USER+1004 // cuando se esconde una barra de herramientas
# define	WM_JINFO	WM_USER+1005

// mensajes del boton de colores
#define CPN_SELCHANGE        WM_USER + 1006        // Colour Picker Selection change
#define CPN_DROPDOWN         WM_USER + 1007        // Colour Picker drop down
#define CPN_CLOSEUP          WM_USER + 1008        // Colour Picker close up
#define CPN_SELENDOK         WM_USER + 1009        // Colour Picker end OK
#define CPN_SELENDCANCEL     WM_USER + 1010        // Colour Picker end (cancelled)

// lista arbol, cuando se ordena un resultado
# define WM_SORTTREE WM_USER+1011

// mensaje de la lista de control
# define	WM_LISTANOTIFICACION	WM_USER+1012 // cuando se selecciona algo en el combobox
# define	WM_LISTACOMBO		WM_USER+1013 // cuando cambia el contenido del combobox

// hilo de importacion
# define WM_IMPORTFILE WM_USER + 1014

// estado de la barra de tareas
# define	WM_SETSTATUS		WM_USER+1015

// notificacion a la barra de estado.
# define	WM_STATUSBAR		WM_USER+1016

// mensaje del conector mysql
# define	WM_MYSQL		WM_USER+1017  // resultados
# define	WM_TREADRESULT		WM_USER+1018  // notificacion del hilo
// notificacion de cuando un archivo cambio
# define	WM_FILECHANGED		WM_USER+1019

// mensaje del conector ODBC
# define	WM_ODBC				WM_USER+1020 // resultados
# define	WM_QUERYCONECTOR	WM_USER+1021 // conector
# define	WM_TREENAVEGADOR	WM_USER+1022 // navegacion
# define 	WM_QUERYTIME		WM_USER+1023 // tiempo del servidor
# define 	WM_REGISTRARUSUARIO	WM_USER+1024 // registrar usuario
# define 	WM_INACTIVIDAD		WM_USER+1024 // inactividad

# define 	WM_CLOSEEX		WM_USER+1025	// terminar la vista actual
# define 	WM_QUERYUSUARIO		WM_USER+1026	// solicitar el usuario.
# define	WM_QUERYFINGER		WM_USER+1027	// solicitar el manejador de lectores
# define	WM_LISTANOTIFICACIONEX	WM_USER+1028 // cuando se selecciona algo en el combobox
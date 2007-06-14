param(
    [string]$BinaryDirectory = 'bin\Release',
    [ValidateRange(1,65535)][int]$Port = 33359
)

$ErrorActionPreference = 'Stop'
$binaries = (Resolve-Path -LiteralPath $BinaryDirectory).ProviderPath
$appName = 'Perfiles'
$profiles = @('Software\SDG\Template', 'Software\SDG\Perfiles')
$currentUser = [Microsoft.Win32.Registry]::CurrentUser
$machine = [Microsoft.Win32.RegistryKey]::OpenBaseKey(
    [Microsoft.Win32.RegistryHive]::LocalMachine, [Microsoft.Win32.RegistryView]::Registry32)
try {
    foreach ($profile in $profiles) {
        foreach ($hive in @($currentUser, $machine)) {
            $existing = $hive.OpenSubKey($profile)
            if ($existing) {
                $existing.Dispose()
                throw 'Use a Windows test account without an existing profile for this program.'
            }
        }
    }
}
finally { $machine.Dispose() }

$exe = Get-Item -LiteralPath (Join-Path $binaries 'Template.exe')
Get-Item -LiteralPath (Join-Path $binaries 'libmysql.dll') | Out-Null
$modules = Get-ChildItem -LiteralPath $binaries -File | Where-Object {
    $_.Extension -in '.exe', '.dll' -and $_.Name -ne 'libmysql.dll'
}
$mark = [guid]::NewGuid().ToString('N')
$created = @()
try {
    foreach ($profile in $profiles) {
        $key = $currentUser.CreateSubKey($profile)
        try { $key.SetValue('__demo_profile', $mark) } finally { $key.Dispose() }
        $created += $profile
    }
    $root = $currentUser.OpenSubKey("Software\SDG\$appName", $true)
    try {
        $server = $root.CreateSubKey('conector\servidor0')
        try {
            $server.SetValue('servidor', '127.0.0.1')
            $server.SetValue('db', 'perfiles_demo')
            $server.SetValue('puerto', $Port, [Microsoft.Win32.RegistryValueKind]::DWord)
            $server.SetValue('incrustado', 0, [Microsoft.Win32.RegistryValueKind]::DWord)
        }
        finally { $server.Dispose() }
        $checksums = $root.CreateSubKey('llaves')
        try {
            foreach ($module in $modules) {
                $digest = [System.Security.Cryptography.IncrementalHash]::CreateHash(
                    [System.Security.Cryptography.HashAlgorithmName]::MD5)
                try {
                    $digest.AppendData([Text.Encoding]::ASCII.GetBytes('10rd'))
                    $digest.AppendData([IO.File]::ReadAllBytes($module.FullName))
                    $digest.AppendData([Text.Encoding]::ASCII.GetBytes('10rdJuHU'))
                    $checksum = [Convert]::ToHexString($digest.GetHashAndReset()).ToLowerInvariant()
                    $checksums.SetValue($checksum, $module.FullName.ToLowerInvariant())
                }
                finally { $digest.Dispose() }
            }
        }
        finally { $checksums.Dispose() }
    }
    finally { $root.Dispose() }
    $desktop = Start-Process -FilePath $exe.FullName -WorkingDirectory $binaries -PassThru -WindowStyle Normal
    $desktop.WaitForExit()
}
finally {
    foreach ($profile in $created) {
        $key = $currentUser.OpenSubKey($profile)
        if (-not $key) { continue }
        try { $owned = $key.GetValue('__demo_profile') -ceq $mark }
        finally { $key.Dispose() }
        if ($owned) { $currentUser.DeleteSubKeyTree($profile) }
    }
}

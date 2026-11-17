# Gestión de perfiles
[English](#english) · [Español](#español)

## English


Legacy Windows desktop app for managing and searching contact, residence and education profiles

> [!WARNING]
> **Status: Legacy.** Windows MFC archive for local testing.

### Overview

Keep a contact, residential address, work history and studies in one Windows record. Search for a profile in the Navigator and open the record to inspect its related details.

### Features

- Open **Navegador → Búsqueda** to filter by name, surname, location, university or study type. Select a result to inspect its record.
- Open **Personales**, **Residencia** or **Oficina** in the Navigator to view the corresponding part of a profile.
- Use **Herramientas → Servidores** to point the app at a separate MySQL test catalog.
- With an administrator account in the test database, choose **Archivo → Importar** to select a CSV file, apply the column mapping and rebuild the search index. Copy `demo/extras/alias.xml` to `bin/Release/extras/alias.xml` before importing fictional records or enabling the **Todo** search columns. The supplied account has the consultation role.
- **Archivo → Administrador** is enabled for group 2 only when the app uses its embedded database. The search demo uses a separate MySQL server. The embedded administrator dialog requires an operator-provided `PERFILES_EMBEDDED_ADMIN_PASSWORD` in the desktop process environment. It rejects elevation when unset. The external MySQL login still uses its own account and group.

### Requirements

See the [third-party notices](THIRD-PARTY-NOTICES.md) for provenance, original notices and hashes.

Verified on Windows 11 build 26300 with Visual Studio Community 18.11.4, MSVC 14.51.36263 (v145), MFC x86, Windows SDK 10.0.26100.0, PowerShell 7.6.9, Docker Engine 29.9.2 and Compose 5.6.1. The archived MySQL 5.0.16 Win32 client and headers are included in vendor/mysql-sdk/. The standard build leaves the archived serial and trial checks under `ENABLE_LICENSE_CHECK`. The recovered Componentes source lacks `Aldia`. `Template/MainFrm.cpp` preserves its installation-path registration using the available library.

### Getting started

Open Developer PowerShell in the checkout. The original MySQL 5.0.16 Win32 client and headers are included under vendor/mysql-sdk/. The archived copies are in vendor/ and template/errorreport/. The notice lists their origin and hashes. The example commands build `bin/Release/Template.exe`, then place its MySQL DLL beside it.

```powershell
$sdk = (Resolve-Path ./vendor/mysql-sdk).Path
msbuild gestion-de-perfiles.sln /m:3 /nr:false /p:Configuration=Release /p:Platform=Win32 /p:PreferredToolArchitecture=x64 "/p:MySqlSdkDir=$sdk"
Copy-Item -LiteralPath (Join-Path $sdk 'libmysql.dll') -Destination 'bin/Release/libmysql.dll'
```

The optional CSV mapping goes beside the executable in its `extras/` folder.

### Usage

Run the launcher on a Windows test account without settings for this application in the current user or machine registry. It creates and removes the temporary application keys.

Start the isolated MySQL fixture with the first command. Run `pwsh -File demo/ProbarEscritorio.ps1` to start the application with temporary application settings in the current Windows account. Use fictional user `consulta_demo` and password `DEMO_LOCAL_USER`. In Search, enter Alicia to locate the fictional profile Alicia Ejemplo (`DEMO-501`). Close the application so the launcher can remove those temporary application settings, then stop the database with the second command.

```powershell
docker compose -f demo/compose.yaml up -d --wait
docker compose -f demo/compose.yaml down --volumes
```

### Project structure

- `Template/Template.vcxproj` builds the application. Its search view is `Template/BusquedaView.cpp`.
- `Administrador/Administrador.vcxproj` builds the user and permissions module.
- `appt/appt.cpp` supplies the application identity and resource entry points.
- `vendor/include/` contains the entry projects for Componentes, Conector, IOFile, LAppc, JLang and JPrint.
- `compat/tmschema.h` points the old theme include at the current Windows SDK.
- `demo/compose.yaml` starts the sample database. `demo/ProbarEscritorio.ps1` launches the app.
- `english/english.vcxproj` and `german/german.vcxproj` build the translated resource DLLs.
- `demo/extras/alias.xml` maps CSV column labels to database fields for the importer.
- `demo/extras/alias-archivada.xml` preserves the July mapping. The demo `alias.xml` uses the single education area column in the July database schema.

### Support

Include the commit, build configuration and exact failing step in a compatibility report. Use invented data for logs and screenshots.

### Contributing

Keep archived behavior and changes in separate commits. Retain external notices and use the isolated sample database for any new demo.

### History

**17 November 2026.** Document the local source folders and the relative location of the CSV mapping. This documentation describes the July 2007 sources.

### Verification

Build and test on 19 November 2026 with MSBuild 18.11.1.51303, MSVC 14.51.36263 (v145) in Release Win32, the MySQL 5.0.16 x86 client and MySQL 5.7.44 in Docker. The app opens the fictional login and returns the DEMO-501 profile from the local database. MSBuild returned exit 0. The commands are in Getting started and Usage. The run exercised login and search. CSV import and Flash rendering were outside this execution. The sample uses 10 tables from the archived application schema plus the connector login tables.

### License

Own sources follow the [MIT License](LICENSE). The [external notices](THIRD-PARTY-NOTICES.md) identify separate components.

---

## Español


Aplicación de escritorio legada para Windows para gestionar y buscar fichas de contacto, residencia y estudios

> [!WARNING]
> **Estado: Legado.** Archivo MFC para pruebas locales.

### Descripción general

Reúne contacto, residencia, información de trabajo y estudios en una ficha de Windows. Busca la ficha en el Navegador y ábrela para consultar sus datos.

### Funciones

- Abre **Navegador → Búsqueda** y filtra por nombre, apellido, ubicación, universidad o tipo de estudio. Selecciona el resultado para consultar su ficha.
- Abre **Personales**, **Residencia** u **Oficina** en el Navegador para consultar los datos de ese apartado.
- Usa **Herramientas → Servidores** para dirigir el programa a un catálogo MySQL de prueba.
- Con una cuenta de administración en la base de prueba, abre **Archivo → Importar** para elegir el CSV, aplicar la relación de columnas e indexar los registros. Copia `demo/extras/alias.xml` a `bin/Release/extras/alias.xml` antes de importar registros ficticios o activar las columnas de búsqueda **Todo**. La cuenta de ejemplo tiene el perfil de consulta.
- **Archivo → Administrador** se habilita para el grupo 2 cuando la aplicación usa su base incrustada. La demostración de búsqueda usa un MySQL separado. El diálogo de administrador incrustado requiere `PERFILES_EMBEDDED_ADMIN_PASSWORD` configurada por el operador en el entorno del proceso. Sin ese valor rechaza la elevación. El ingreso a MySQL externo conserva su cuenta y grupo propios.

### Requisitos

Consulta los [avisos de terceros](THIRD-PARTY-NOTICES.md) para ver el origen, los avisos originales y los hashes.

Probado en Windows 11 compilación 26300 con Visual Studio Community 18.11.4, MSVC 14.51.36263 (v145), MFC x86, Windows SDK 10.0.26100.0, PowerShell 7.6.9, Docker Engine 29.9.2 y Compose 5.6.1. El cliente y las cabeceras MySQL 5.0.16 Win32 del respaldo están en vendor/mysql-sdk/. La compilación normal conserva el serial y el periodo de prueba archivados bajo `ENABLE_LICENSE_CHECK`. Los fuentes recuperados de Componentes carecen de `Aldia`. `Template/MainFrm.cpp` conserva el registro de la ruta de instalación con la biblioteca disponible.



### Cómo empezar

Abre Developer PowerShell desde la raíz. El cliente MySQL 5.0.16 Win32 y sus cabeceras están en vendor/mysql-sdk/. Las copias del respaldo están en vendor/ y template/errorreport/. Los avisos enumeran sus orígenes y hashes. Los comandos generan `bin/Release/Template.exe` y después colocan junto a él la DLL de MySQL.

```powershell
$sdk = (Resolve-Path ./vendor/mysql-sdk).Path
msbuild gestion-de-perfiles.sln /m:3 /nr:false /p:Configuration=Release /p:Platform=Win32 /p:PreferredToolArchitecture=x64 "/p:MySqlSdkDir=$sdk"
Copy-Item -LiteralPath (Join-Path $sdk 'libmysql.dll') -Destination 'bin/Release/libmysql.dll'
```

La correspondencia opcional del CSV va junto al ejecutable en su carpeta `extras/`.

### Uso

Ejecuta el lanzador en una cuenta de prueba de Windows sin configuración previa de esta aplicación en el Registro del usuario ni del equipo. El lanzador crea y retira las claves temporales del programa.

Inicia el MySQL aislado con el primer comando. Ejecuta `pwsh -File demo/ProbarEscritorio.ps1` para abrir la aplicación con una configuración temporal de la aplicación en la cuenta de Windows. Usa el usuario ficticio `consulta_demo` y la contraseña `DEMO_LOCAL_USER`. En Búsqueda, escribe Alicia para localizar la ficha ficticia Alicia Ejemplo (`DEMO-501`). Cierra la aplicación para que el lanzador retire esa configuración temporal. Luego detén la base con el segundo comando.

```powershell
docker compose -f demo/compose.yaml up -d --wait
docker compose -f demo/compose.yaml down --volumes
```

### Estructura del proyecto

- `Template/Template.vcxproj` compila la aplicación. La vista de búsqueda está en `Template/BusquedaView.cpp`.
- `Administrador/Administrador.vcxproj` compila el módulo de usuarios y permisos.
- `appt/appt.cpp` suministra la identidad y los puntos de acceso a los recursos.
- `vendor/include/` contiene los proyectos de Componentes, Conector, IOFile, LAppc, JLang y JPrint.
- `compat/tmschema.h` dirige el encabezado de temas al SDK actual de Windows.
- `demo/compose.yaml` inicia la base de ejemplo. `demo/ProbarEscritorio.ps1` abre el programa.
- `english/english.vcxproj` y `german/german.vcxproj` compilan las DLL de recursos traducidos.
- `demo/extras/alias.xml` vincula las etiquetas de las columnas del CSV con los campos de la base para importar.
- `demo/extras/alias-archivada.xml` conserva la correspondencia de julio. El `alias.xml` de ejemplo usa la columna única de área de estudios del esquema de julio.

### Soporte

Incluye el commit, la configuración de compilación y el paso exacto que falló en tu informe. Usa datos ficticios en registros y capturas.

### Contribuir

Conserva el comportamiento histórico y las revisiones nuevas en commits separados. Mantén los avisos externos y usa la base de ejemplo aislada en las demostraciones.

### Historial

**17 de noviembre de 2026.** Documenta las carpetas de fuentes incluidas y la ruta de la correspondencia CSV. La documentación corresponde a las fuentes de julio de 2007.

### Verificación

Compilación y prueba del 19 de noviembre de 2026 con MSBuild 18.11.1.51303, MSVC 14.51.36263 (v145) en Release Win32, el cliente MySQL 5.0.16 x86 y MySQL 5.7.44 en Docker. La aplicación inicia sesión con la cuenta ficticia y devuelve el perfil DEMO-501 de la base local. MSBuild terminó con código 0. Los comandos están en Cómo empezar y Uso. La ejecución comprobó el ingreso y la búsqueda. La importación CSV y Flash quedaron fuera de esa prueba. El ejemplo usa 10 tablas del esquema archivado más las tablas de ingreso del conector.

### Licencia

Las fuentes propias usan la [licencia MIT](LICENSE). Los [avisos externos](THIRD-PARTY-NOTICES.md) identifican los componentes separados.

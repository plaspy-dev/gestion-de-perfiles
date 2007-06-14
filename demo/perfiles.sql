SET SESSION sql_mode='';
SET NAMES utf8mb4;
CREATE TABLE IF NOT EXISTS shared (

	id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	nombre VARCHAR(255) NOT NULL,
	descripcion TEXT,
	nReservado INT,
	szReservado TEXT,
	PRIMARY KEY(id),
	INDEX name (nReservado)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS usuario (
	pkz VARCHAR(255) NOT NULL,
	titulo VARCHAR(255),
	nombre VARCHAR(255),
	apellido VARCHAR(255),
	sapellido VARCHAR(255),
	nacimiento DATE,
	sexo INT,
	nacionalidad VARCHAR(255),
	correspondencia INT,
	estado INT,
	fallecido INT,
	PRIMARY KEY(pkz)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS residencia (
	pkz VARCHAR(255) NOT NULL,
	pais VARCHAR(255),
	departamento VARCHAR(255),
	ciudad VARCHAR(255),
	direccion VARCHAR(255),
	cpostal VARCHAR(255),
	aaereo VARCHAR(255),
	telefono VARCHAR(255),
	fax VARCHAR(255),
	email VARCHAR(255),
	email2 VARCHAR(255),
	website VARCHAR(255),
	movil VARCHAR(255),
	PRIMARY KEY(pkz)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS empresa (
	pkz VARCHAR(255) NOT NULL,
	nombre VARCHAR(255),
	pais VARCHAR(255),
	departamento VARCHAR(255),
	ciudad VARCHAR(255),
	direccion VARCHAR(255),
	cpostal VARCHAR(255),
	aaereo VARCHAR(255),
	telefono VARCHAR(255),
	fax VARCHAR(255),
	email VARCHAR(255),
	website VARCHAR(255),
	cargo VARCHAR(255),
	tipo INT,
	PRIMARY KEY(pkz)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS educacion (
	pkz VARCHAR(255) NOT NULL,
	carrera VARCHAR(255),
	universidad VARCHAR(255),
	comienzob1 DATE,
	finb1 DATE,
	comienzob2 DATE,
	finb2 DATE,
	revista VARCHAR(255),
	titulos VARCHAR(255),
	tipo1 VARCHAR(255),
	tipo2 VARCHAR(255),
	becario INT,
	PRIMARY KEY(pkz)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS estudios (
	pkz VARCHAR(255) NOT NULL,
	pregrado VARCHAR(255),
	puniversidad VARCHAR(255),
	maestria VARCHAR(255),
	muniversidad VARCHAR(255),
	doctorado VARCHAR(255),
	duniversidad VARCHAR(255),
	otros VARCHAR(255),
	ouniversidad VARCHAR(255),
	parea INT,
	marea INT,
	darea INT,
	oarea INT,
	PRIMARY KEY(pkz)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS estudiosfuera (
	pkz VARCHAR(255) NOT NULL,
	pregrado VARCHAR(255),
	puniversidad VARCHAR(255),
	ppais VARCHAR(255),
	maestria VARCHAR(255),
	muniversidad VARCHAR(255),
	mpais VARCHAR(255),
	doctorado VARCHAR(255),
	duniversidad VARCHAR(255),
	dpais VARCHAR(255),
	otros VARCHAR(255),
	ouniversidad VARCHAR(255),
	opais VARCHAR(255),
	parea INT,
	marea INT,
	darea INT,
	oarea INT,
	PRIMARY KEY(pkz)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS seguimiento (
	pkz VARCHAR(255) NOT NULL,
	primero DATE,
	mmedio VARCHAR(255),
	segundo DATE,
	smedio VARCHAR(255),
	tercero DATE,
	tmedio VARCHAR(255),
	encuesta DATE,
	respondio VARCHAR(255),
	PRIMARY KEY(pkz)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS gestion (
	pkz VARCHAR(255) NOT NULL,
	actualizado INT,
	actualizadopor VARCHAR(255),
	ultimaactualizacion DATETIME,
	notas TEXT,
	PRIMARY KEY(pkz)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS busqueda (
	pkz VARCHAR(255) NOT NULL,
	texto TEXT,
	PRIMARY KEY(pkz),
	FULLTEXT(pkz,texto)
) ENGINE=MyISAM;

CREATE TABLE grupo (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nombre VARCHAR(120) NOT NULL,
    descripcion VARCHAR(160)
);
CREATE TABLE permisos (
    grupo INT NOT NULL,
    clase VARCHAR(80) NOT NULL,
    permiso INT NOT NULL,
    PRIMARY KEY (grupo, clase)
);
CREATE TABLE usuarios (
    id INT PRIMARY KEY AUTO_INCREMENT,
    login VARCHAR(80) NOT NULL UNIQUE,
    nombre VARCHAR(120) NOT NULL,
    descripcion VARCHAR(160),
    grupo INT NOT NULL,
    vigencia DATETIME NULL,
    expira INT DEFAULT 0,
    ucambio DATETIME NULL,
    acceso DATETIME NULL,
    autonumerico INT DEFAULT 0,
    estado INT DEFAULT 1
);
CREATE TABLE usuariosh (
    usuario INT NOT NULL,
    dia INT NOT NULL,
    entrada INT NOT NULL,
    salida INT NOT NULL,
    PRIMARY KEY (usuario, dia, entrada)
);

INSERT INTO usuario (pkz, nombre, apellido, titulo)
VALUES ('DEMO-501','Alicia','Ejemplo','Perfil ficticio');
INSERT INTO grupo (id,nombre,descripcion) VALUES (2,'Consulta demo','Grupo ficticio');
INSERT INTO permisos (grupo,clase,permiso) VALUES
  (2,'CBusquedaView',1),(2,'CEstudiosView',1),(2,'CTemplateView',1);
INSERT INTO usuarios (id,login,nombre,descripcion,grupo)
VALUES (401,'consulta_demo','Operador ficticio','0',2);
INSERT INTO shared (id,nombre,descripcion,nReservado,szReservado)
VALUES (401,'consulta_demo','0',2,'2');
INSERT INTO busqueda (pkz,texto) VALUES ('DEMO-501','Alicia Ejemplo perfil ficticio');

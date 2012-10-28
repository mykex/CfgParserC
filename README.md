Configuration File Parser Library
===================================

> Miguel Abreu
> v1: 2010, v2: 2012, v3: 2012

<br><br>
Init Library calling ConfigOpen(file)
* file - Configuration file path

<br><br>
Use ConfigGetParam(type, name, value) to get a param value
* type - Param type (1-String, 2-Numeric, 3-Boolean)
* name - Param exact name in configuration file (e.g.: 'param1=')
* value - Location where param value is stored, if found

<br><br>
Call ConfigClose() after reading all parameters value

<br><br>
NOTES
=====
* DO NOT use parameters longer than 256 chars !
* Use non-spaced parameters e.g.: 'timeout=100' or 'serial=/dev/tty0
* The code discards all lines that begin with '#' char

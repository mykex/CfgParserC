/* -- CONFIGURATION FILE PARSER LIBRARY
 * Miguel Abreu
 *        v1: 2010
 *        v2: 2012
 *        v3: 2012
 * 
 * config.c - Main File
 * config.h - Header file
 * 
 * - Init Library calling ConfigOpen(file)
 *     file - Configuration file path
 * - Use ConfigGetParam(type, name, value) to get a param value
 *     type - Param type (1-String, 2-Numeric, 3-Boolean)
 *     name - Param exact name in configuration file (e.g.: 'param1=')
 *     value - Location where param value is stored, if found
 * - Call ConfigClose() after reading all parameters value
 *
 * - DO NOT use parameters longer than 256 chars !
 * - Use non-spaced parameters e.g.: 'timeout=100' or 'serial=/dev/tty0
 * - The code discards all lines that begin with '#' char
*/
/** @file config.h
    @brief Configuration file parser library
*/
#ifndef CONFIG_H
#define CONFIG_H

/** @fn int ConfigGetParam(int type, const char *name, void *value)
    @brief Try to get a param value from configuration file. Call ConfigOpen() before using this function
    @param type Param type: 1-String, 2-Numeric, 3-Boolean
    @param name Param exact name in configuration file (e.g.: 'param1=')
    @param value Location where param value is stored, if found
    @return True if param is found, False otherwise
*/
int ConfigGetParam(int type, const char *name, void *value);
/** @fn ConfigOpen(char *file)
    @brief Open configuration file handler. Call this function once per program execution
    @param file File name
    @return Zero or negative error code
*/
int ConfigOpen(char *file);
/** @fn void ConfigClose()
    @brief Close configuration file handler
*/
void ConfigClose();

#endif

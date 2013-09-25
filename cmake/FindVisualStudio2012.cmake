# This module defines
# VS_DEVENV, path to devenv.com.

FIND_PATH(VS110_DIR devenv.com
  $ENV{VS110COMNTOOLS}/../IDE
  "C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\Common7\\IDE"
  "C:\\Program Files\\Microsoft Visual Studio 11.0\\Common7\\IDE"
  "C:\\Programme\\Microsoft Visual Studio 11.0\\Common7\\IDE"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\11.0\\Setup\\VS;EnvironmentDirectory]"
)

SET(VS110_FOUND 0)
IF(VS110_DIR)
  SET(VS110_FOUND 1)
  MESSAGE(STATUS "Found Visual Studio 2012")
ENDIF(VS110_DIR)

MARK_AS_ADVANCED(
  VS110_DIR
)

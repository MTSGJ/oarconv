#ifndef  __JBXL_LOG_BASE_TOOL_H_
#define  __JBXL_LOG_BASE_TOOL_H_

/**
Log Document用ツール

*/


#ifdef WIN32

#ifdef  PRINT_MESG
#undef  PRINT_MESG
#define PRINT_MESG  if(GLTextDocument!=NULL)GLTextDocument->printFormat
#endif

#endif  // WIN32


#endif  // _JBXL_OAR_BASE_TOOL_H_

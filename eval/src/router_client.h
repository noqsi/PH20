/*
 * $Header: /usr/hete/repository/infra/cns/router_client.h,v 1.1 1997/11/20 16:19:03 jimf Exp $
 * $Log: router_client.h,v $
 * Revision 1.1  1997/11/20 16:19:03  jimf
 * Moved over from ground/comm/cns.
 * missing_prototypes.h is a copy of the former libc_sunos4.h
 *
 * Revision 1.1  1997/10/20 16:38:25  jimf
 * Checkpoint HETE-I files
 *
 * Revision 1.1  1994/05/18  15:02:06  jpd
 * Initial revision
 *
 */

int router_connect( const char * routerstring );
const char *router_string( void );

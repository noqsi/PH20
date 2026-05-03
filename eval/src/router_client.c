/*
 * $Header: /usr/hete/repository/infra/cns/router_client.c,v 1.2 1999/05/13 21:16:07 jimf Exp $
 *
 * $Log: router_client.c,v $
 * Revision 1.2  1999/05/13 21:16:07  jimf
 * Added setsockopt to enable SO_REUSEADDR, allowing faster reuse of the socket
 * address if the program dies in ugly ways.
 *
 * Revision 1.1  1997/11/20 16:19:02  jimf
 * Moved over from ground/comm/cns.
 * missing_prototypes.h is a copy of the former libc_sunos4.h
 *
 * Revision 1.3  1997/11/05 22:39:06  rosalia
 * * Changes to comm
 *
 * ** The ground communications package (CNS, libipp.a) now works on
 *    sunos4, solaris, intel linux and sparc linux.
 *
 * ** The ground communications package (CNS, libipp.a) now supports the
 *    next generation IPP, in which length and priority are collapsed
 *    into a single 16-bit IPP_word, and the subtype is gone.
 *
 * I also added ./NEWS and comm/ChangeLog and comm/cns/ChangeLog files,
 * so the changes are described in more detail in the ChangeLog files.
 *
 * Revision 1.2  1997/10/27 17:37:50  jpd
 * Use portable ipw.h and ipp.h.
 * Convert to IP3 headers, new routing messages (no subtypes).
 * Revert to old style, uncovering some bugs.
 *
 * Revision 1.1  1997/10/20 16:38:24  jimf
 * Checkpoint HETE-I files
 *
 * Revision 1.3  1994/10/13  16:05:57  gbc
 * gethostbyaddr was a mistake, as it still reads /etc/hosts....
 *
 * Revision 1.2  1994/10/12  18:51:44  gbc
 * now uses libc.h
 * collected socket code together
 * turned on close on exec
 * now recognizes constellation host by address
 *
 * Revision 1.1  1994/05/18  14:59:59  jpd
 * Initial revision
 *
 */

/* libc.h is really only used for legacy OSes (sunos4, old nextstep) */

#ifdef HAVE_LIBC_H
#include <libc.h>
#endif /* HAVE_LIBC_H */

#ifndef STDC_HEADERS
# include "missing_prototypes.h"
#endif /* STDC_HEADERS */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "router_client.h"
#include "CNS.h"

extern unsigned long inet_addr(char *cp);

int router_connect( const char * routerstring )
{
	char            consthost[128];
	unsigned        port = CNS_DEFAULT_PORT;
	struct sockaddr_in name;
	int             namelen = sizeof (name);
	struct hostent	*host;
	int		cns;
	unsigned        opt;

	consthost[0] = '\0';
	strncat (consthost, CNS_DEFAULT_HOST, sizeof (consthost) - 1);
	if (routerstring) {
	    char           *dot;

	    sscanf (routerstring, " %s ", consthost);
	    dot = rindex (consthost, '.');
	    if (dot) {
		*dot = '\0';
		port = atoi (++dot);
	    } else {
		fprintf (stderr, "IPP_CONSTELLATION must be host.port\n");
		exit (1);
	    }
	}
	host = gethostbyname (consthost);
	name.sin_family = PF_INET;
	name.sin_port = htons (port);
	name.sin_addr.s_addr = (host)
		? *((unsigned long *) (host->h_addr_list[0]))
		: inet_addr(consthost);
	if (name.sin_addr.s_addr == (unsigned long)(-1)) {
		fprintf( stderr, "Unknown host: %s\n", consthost );
		exit( 1 );
	}
	/*
	 * connect to cns, setting close on exec so that only
	 * the parent holds process slot if children are spawned
	 */
	if ((cns = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    perror ("IPP_register_address: socket");
	if (-1 == fcntl(cns, F_SETFD, 1))
	    perror ("IPP_register_address: fcntl");
	opt = 1;
	if (setsockopt(cns, SOL_SOCKET, SO_REUSEADDR,
		       (char*)&opt, sizeof(opt)) < 0)
	    perror("IPP_register_address: setsockopt");
	if (connect (cns, (struct sockaddr *)&name, namelen))
	    perror ("IPP_register_address: connect");
	
	return cns;
}

const char *router_string( void )
{
	char *envinfo = getenv ("IPP_CONSTELLATION");
	return envinfo;
}

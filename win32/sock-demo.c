#define WINVER 0x0502
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char *getaddrbyname(char*);

int main(int argc, char *argv[])
{
	char host[255] = "localhost";
	char uri[255] = "/";
	char addr[255];
	char httphead[1024];
	char buffer[1024];
	char *psubstr;
	char http_useragent[] = "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.1) Gecko/20090624 Firefox/3.5\r\n";
	char http_accept[] = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
	char http_lang[] = "Accept-Language: zh-cn,zh;q=0.5\r\n";
	//char http_encode[] = "Accept-Encoding: gzip,deflate\r\n";
	char http_encode[] = "";
	char http_charset[] = "Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7\r\n";
	char http_alive[] = "Keep-Alive: 300\r\n";
	char http_conn[] = "Connection: keep-alive\r\n";

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	int is_first = 1;
	SOCKET s;
	struct sockaddr_in addr_in;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
		return;
	if(argc > 1)
		strcpy(host,argv[1]);
	if(argc > 2)
		strcpy(uri,argv[2]);
	strcpy(addr, getaddrbyname(host) );
	sprintf(httphead, "GET %s HTTP/1.1\r\nHost: %s\r\n%s%s%s%s%s%s%s\r\n", uri, host, http_useragent, http_accept, http_lang, http_encode, http_charset, http_alive, http_conn);
	s = socket(AF_INET, SOCK_STREAM, 0);
	memset((void*)&addr_in.sin_zero, 0, 8);
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(80);
	addr_in.sin_addr.s_addr = inet_addr(addr);

	connect(s, (struct sockaddr*)&addr_in, sizeof(addr_in));
	send(s, httphead, sizeof(httphead), 0);
	while(1)
	{
			memset(buffer, 0, 1024);
			if(!(recv(s, buffer, 1023, 0))) break;
			if(is_first)
			{
					is_first = 0;
					psubstr = strstr(buffer, "\r\n\r\n");
					if(psubstr==NULL) break;
					strcpy(buffer, psubstr+4);
			}
			printf("%s", buffer);
	}
	shutdown(s, 0);
	WSACleanup();

	return 0;
}

char *getaddrbyname(char *hostname)
{
	struct hostent *h_hostent;
	h_hostent = gethostbyname(hostname);
	return inet_ntoa( *((struct in_addr*) h_hostent->h_addr) );
}
#include "utility.h"

int initServer(int port, int buf_size, int max_pending){
	int retval;
	int serv_sock;
	char buf[buf_size], tmp[20];
	struct sockaddr_in serveraddr;
	struct timeval timeout;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // TCP socket
	if(serv_sock == -1) return -1;

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;
	setsockopt(serv_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);

	retval = bind(serv_sock, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
	if(retval == -1) return -2;

	retval = listen(serv_sock, max_pending);
	if(retval == -1) return -3;

	return serv_sock;
}

void err_quit(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int kbhit()
{
	struct termio stTerm;
	u_short usFlag;
	u_char uchMin;
	u_char uchTime;
	char szBuf[10];

	// ǥ�� �Է� �����ľ�
	ioctl( 0, TCGETA, &stTerm );

	// �� ����
	usFlag= stTerm.c_lflag;
	uchMin= stTerm.c_cc[ VMIN ];
	uchTime= stTerm.c_cc[ VTIME ];

	// low ���� ����
	stTerm.c_lflag &= ~ICANON;
	// readȣ��� 0������ �о����
	stTerm.c_cc[ VMIN ] = 0;
	// �ð� ���� ����
	stTerm.c_cc[ VTIME ]= 0;

	// ���� ����
	ioctl( 0, TCSETA, &stTerm );

	// read() ȣ��
	if( read( 0, szBuf, 9 ) <= 0 )
	{
		// �����·� ����
		stTerm.c_lflag = usFlag;
		stTerm.c_cc[ VMIN ] = uchMin;
		stTerm.c_cc[ VTIME ]= uchTime;
		ioctl( 0, TCSETA, &stTerm );

		// Ű�� �ȴ�������
		return 0;
	}
	else
	{
		// �����·� ����
		stTerm.c_lflag = usFlag;
		stTerm.c_cc[ VMIN ]= uchMin;
		stTerm.c_cc[ VTIME ]= uchTime;
		ioctl(0, TCSETA, &stTerm);

		// Ű�� ���������� �˸�
		return 1;
	}
}

int getch(){
	int ch;
	struct termios oldt, newt;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

void printIP(){
	// �̴��� ������ ����ü
	struct ifreq *ifr;
	struct sockaddr_in *sin;
	struct sockaddr *sa;

	// �̴��� ���� ����ü
	struct ifconf ifcfg;
	int fd;
	int n;
	int numreqs = 30;
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	// �̴��� ���������� ��������� ���ؼ�
	// ���� ����ü�� �ʱ�ȭ�ϰ�
	// ifreq�����ʹ� ifc_buf�� ����Ǹ�,
	// ��Ʈ��ũ ��ġ�� ������ ���� �� �����Ƿ� ũ�⸦ ����� ����־�� �Ѵ�.
	// ������ �������ּҿ� �ϳ��� �̴���ī��, 2���� ��ġ�� ������.
	memset(&ifcfg, 0, sizeof(ifcfg));
	ifcfg.ifc_buf = NULL;
	ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;
	ifcfg.ifc_buf = malloc(ifcfg.ifc_len);

	for (;;) {
		/*        ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;
		 ifcfg.ifc_buf = realloc(ifcfg.ifc_buf, ifcfg.ifc_len);*/
		if (ioctl(fd, SIOCGIFCONF, (char *) &ifcfg) < 0) {
			perror("SIOCGIFCONF ");
			exit;
		}
		break;
	}

	ifr = ifcfg.ifc_req;

	for (n = 0; n < ifcfg.ifc_len; n += sizeof(struct ifreq)) {
		sin = (struct sockaddr_in *) &ifr->ifr_addr;
		printf("IP    %s\n", inet_ntoa(sin->sin_addr));
		ifr++;
	}

	close(fd);
}

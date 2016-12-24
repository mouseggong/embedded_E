#include "CServer.h"
int client_sock2, client_sock;
int clientSockfd2;
int program_run = 1; // ���α׷� ���� �� ��� �����带 �����ϱ� ���� �÷���
int client_cnt = 0; // Ŭ���̾�Ʈ�� ���� ī��Ʈ
int heartbeat;
int main(int argc, char * argv[]) {
	pthread_t thread_id;
	struct sockaddr_in client_addr;
	struct sockaddr_in client_addr2;
	int server_sock;
	int server_sock2;
	int addr_len;

	printIP();

	//initServer(port, buffer_size, max_pending)
	server_sock = initServer(7777, BUFSIZE, 5);
	server_sock2 = initServer(7780, BUFSIZE, 5);

	if(server_sock==-1) err_quit("socket() error");
	else if(server_sock==-2) err_quit("bind() error");
	else if(server_sock==-3) err_quit("listen() error");
	if(server_sock2==-1) err_quit("socket() error");
	else if(server_sock2==-2) err_quit("bind() error");
	else if(server_sock2==-3) err_quit("listen() error");
	printf("Server Started\n");

	while(1)
	{
		//�ƹ�Ű�� ������ ���� ����
		if(kbhit()) break;

		//Ŭ���̾�Ʈ ���� ��ٸ� (1�ʸ��� üũ)
		addr_len = sizeof(client_addr);
		client_sock = accept(server_sock, (struct sockaddr*) &client_addr, &addr_len);
		addr_len = sizeof(client_addr2);
		client_sock2 = accept(server_sock2, (struct sockaddr*) &client_addr2, &addr_len);
//		printf("client_sock = %d\n",client_sock);
//		printf("client_sock2 = %d\n",client_sock2);
		// Ŭ���̾�Ʈ�� �������� ��� ��� ������ ����
		if(client_sock2>0){
			printf("that is good good good\n");
			clientSockfd2=client_sock2;
		}
		if(client_sock>0){
			pthread_create(&thread_id, 0, recvThread, (void*)client_sock);
		}
	}

	printf("Server Closed\n");

	program_run = 0;

	close(server_sock);
}

void printPacketInfo(unsigned char* packet){
	int id = packet[3];
	int cds = (packet[4]<<8)|packet[5];
	int temp = packet[6];
	int menu = packet[8];
	int i = 0;
	unsigned char csc = packet[14];
	unsigned char sum = 0;
	int emergency = 0;
	unsigned char packet_2[] = {0xA0, 0x0A, 0x00, 0x00, 0x00, 0x00, 255, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xA0};

	if(packet[7] != 0){
		heartbeat = packet[7];
	}

	for(i=2;i<14;i++){
		sum += packet[i];
	}

	if(sum==csc){
		printf("[%03d] ���:%d, �µ�:%d, �ɹ�(BPM):%d, menu:%d\n", id, cds, temp, heartbeat, menu);
	} else {
		printf("[%03d] Checksum error\n", id);
	}
	 if(menu>0){
		 unsigned char csc = 0;
	     packet_2[5] = 255;
	     packet_2[6] = 0;
		 switch(menu){
	        	case 1:
	            	printf("help\n");
	                break;
	            case 2:
	            	printf("Restroom\n");
	                break;
	            case 3:
	            	printf("Thirsty\n");
	            	break;
	            case 4:
	            	printf("Hungry\n");
	            	break;
	            case 5:
	            	printf("Light On\n");
	            	break;
	            case 6:
	            	printf("Light Off\n");
	            	break;
	       }
	   }
	 else{
		 printf("No problem\n");
	 }
	 //if(temp<25 || temp>40){

	 if(temp>40 || temp<20){
			 printf("�µ� ���� �̻� : %d\n", temp);
			 emergency = 1;
	 }
	 else if(temp>37){
		 printf("�µ� ���� �̻� : %d\n", temp);
		 packet_2[4] = 255;
	     packet_2[5] = 255;
	     packet_2[6] = 0;
	 }

	 if(heartbeat != 0 && (heartbeat < 50 || heartbeat>300)){
		 printf("�ɹ� �̻�: %d\n", heartbeat);
		 emergency = 1;
	 }

	 if(emergency == 1){
		 packet_2[4] = 255;
		 packet_2[5] = 0;
		 packet_2[6] = 0;
		 packet_2[7] = 1;
	 }
	 for(i=0;i<17;i++){
		 printf("%d ",(int)packet_2[i]);
	 }
	 send(clientSockfd2, packet_2, sizeof(packet_2), 0);
}

void sendPacket(int clientSockfd, unsigned char* data){

	unsigned char packet[] = {0xA0, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xA0};
	unsigned char csc = 0;
	int i=0;

	if(sizeof(data)!=10){
		printf("sending error\n");
		return;
	}
	printf("in sending\n");
	for(i=0;i<10;i++){
		packet[4+i] = data[i];
		csc += data[i];
	}

	packet[14] = csc;

	send(clientSockfd, packet, sizeof(packet), 0);
}

void * recvThread(void * vpData)
{
	int clientSockfd = (int)vpData;
	int recvLen;
	int i;
	int packet_pt = 0;
	unsigned char recvBuffer[BUFSIZE];
	unsigned char packet_buffer[20];
	unsigned char packet_started = 0;

	printf("Client connected (Total:%d)\n", ++client_cnt);

	sleep(1);
	printf("clientSockfd = %d\n",clientSockfd);
	// ����� ��⿡�� ������ ������ �����϶�� ��Ŷ�� ������.
	if ((send(clientSockfd, SENSOR_REQ_PACKET, sizeof(SENSOR_REQ_PACKET), 0))<0) {
		printf("send() ERROR\n");
		printf("Client disconnected\n");
		close(clientSockfd);
		client_cnt--;
		return 0;
	}

	while (program_run) {
		// clientSockfd�� ������ �����͸� �޾� recvBuffer�� ����
		if ((recvLen = recv(clientSockfd, recvBuffer, BUFSIZE - 1, 0)) < 0) continue;

		// Ŭ���̾�Ʈ�� ������ ���´ٸ� while�� ����
		if (recvLen == 0) break;

		// ��Ŷ ������ ����
		for(i=0;i<recvLen;i++){
			packet_buffer[packet_pt++] = recvBuffer[i];

			if(packet_started){ //��Ŷ�� ����� ���� ���¶�� ���� �����Ͱ� ���������� �Ǵ� ���� ũ�Ⱑ �ʰ��Ǿ����� �˻�
				// ������ ��Ŷ �����Ͷ�� printPacketInfo �Լ� ȣ�� �� �ʱ�ȭ
				if(packet_buffer[packet_pt-2]==0x0A&&packet_buffer[packet_pt-1]==0xA0&&packet_pt==17){
					printPacketInfo(packet_buffer);
					//unsigned char data[10] = {0,0,0,0,0,0,0,0,0,0};
					//sendPacket(clientSockfd, data);
					packet_pt = 0;
					packet_started = 0;
					continue;
				}

				// ���� ũ�Ⱑ �ʰ��Ǿ��ٸ� �ʱ�ȭ
				if(packet_pt>=20){
					packet_pt = 0;
					packet_started = 0;
					printf("BUFFER OVERFLOW ERROR\n");
				}
			} else { //��Ŷ�� ����� ���� ���� ���¶�� ���� �����Ͱ� ������� �˻�
				// ���� 2���� �����Ͱ� ����� �ƴ϶�� �ʱ�ȭ
				// ������ packet_started�� 1�� ����
				if(packet_pt>=2){
					if(packet_buffer[0]==0xA0 && packet_buffer[1]==0x0A){
						packet_started = 1;
					} else {
						packet_pt = 0;
						printf("START PACKET ERROR\n");
					}
				}
			}
		}
	}

	client_cnt--;
	close(clientSockfd);
	printf("Client disconnected\n");

	return 0;
}
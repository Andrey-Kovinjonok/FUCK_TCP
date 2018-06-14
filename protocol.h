#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/*
Packet := header + body
header := length + '|';
length := [0-9]+
body   := text

Example:

01A
12Hello World!
*/

#define MAX_PACKET_SIZE 1024

char* encode_packet(const char *text);
char* parse_packet(struct Buffer *buf);

char* encode_packet(const char *text){
	int body_len = strlen(text);
	if(body_len > MAX_PACKET_SIZE - 15){
		return NULL;
	}
	char header[20];
	int header_len = snprintf(header, sizeof(header), "%d|", body_len);
	int packet_len = header_len + body_len;

	char *buf = (char *)malloc(packet_len + 1);
	snprintf(buf, packet_len + 1, "%s%s", header, text);
	return buf;
}

char* parse_packet(struct Buffer *buf){
	if(buf->size == 0){
		return NULL;
	}
	int head_len;
	int body_len;

	{
		char *body = (char *)memchr(buf->data, '|', buf->size);
		if(body == NULL){
			printf("[Partial Packet] header not ready, buffer %d\n", buf->size);
			return NULL;
		}
		body ++;
		head_len = body - buf->data;
	}

	{
		char header[20];
		memcpy(header, buf->data, head_len - 1); // no '|'
		header[head_len - 1] = '\0';
		body_len = atoi(header);
	}

	if(buf->size < head_len + body_len){
		printf("[Partial Packet] body not ready, buffer %d\n", buf->size);
		return NULL;
	}
	
	char *body = malloc(body_len + 1);
	if(body_len > 0){
		memcpy(body, buf->data + head_len, body_len);
	}
	body[body_len] = '\0';
	
	buffer_del(buf, head_len + body_len);
	
	return body;
}

#endif

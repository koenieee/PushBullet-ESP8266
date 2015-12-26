#ifndef _PUSHBULLET_H_
#define _PUSHBULLET_H_

#include <SPI.h>
#include <WiFiClientSecure.h>


typedef enum {NOTE, LINK, FILES} PUSH_TYPE;



class PushBullet{
	
	public:
	
		PushBullet(const String api_token, WiFiClientSecure * secure_client, uint16_t port);
		
		bool openConnection();
		bool closeConnection();
		bool checkConnection();
		
		void sendNormalPush(const String message, const String title, PUSH_TYPE theType);
		void sendSMSPush();
		void copyToClipboard();
		
	private:
	
		String buildRequest(String url, String body);
	
		WiFiClientSecure * secure_client;
		uint16_t port;
		
		
		String api_token;
		const String push_bullet_host = "api.pushbullet.com";
		
	
	
};


#endif
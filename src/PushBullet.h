#ifndef _PUSHBULLET_H_
#define _PUSHBULLET_H_

#include <SPI.h>
#include <WiFiClientSecure.h>

#define DEBUGGING true

class PushBullet{
	
	public:
	
		PushBullet(const String api_token, WiFiClientSecure * secure_client, uint16_t port);
		bool checkConnection();
		void sendNotePush(const String message, const String title);
		void sendLinkPush(const String message, const String title, const String url);
		
		void sendSMSPush(const String message, const String phoneNumber, const String source_device, const String source_user);
		void copyToClipboard(const String contents, const String source_device = "", const String source_user = "");
		
	private:
	
		String buildRequest(String url, String body);
		bool sendRequest(String reg);
		void sendAndToggl(String req);
		void togglConnection();
	
		bool openConnection();
		bool closeConnection();

	
		WiFiClientSecure * secure_client;
		uint16_t port;
		
		
		String api_token;
		const String push_bullet_host = "api.pushbullet.com";
		
	
	
};


#endif

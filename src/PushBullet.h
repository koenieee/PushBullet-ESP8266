#ifndef _PUSHBULLET_H_
#define _PUSHBULLET_H_

#include <SPI.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#define DEBUGGING true
//#define RETURN_FULLHEADER false

struct pushesJsonObject {
	String title;
	String message;
	String byWho;
	
};

class PushBullet{
	
	public:
	
		PushBullet(const String api_token, WiFiClientSecure * secure_client, uint16_t port);
		
		bool openConnection();
		bool closeConnection();
		bool checkConnection();
		
		String sendNotePush(const String message, const String title);
		String sendLinkPush(const String message, const String title, const String url);
		
		String sendSMSPush(const String message, const String phoneNumber, const String source_device, const String source_user);
		String copyToClipboard(const String contents, const String source_device = "", const String source_user = "");
		
		String registerThisDevice(const String nickName, const String pushToken);
		
		String getLatestPushed(const String modified_after, int limit);
		
		pushesJsonObject convertJsonToStruct(char json[600]);
		
		
	private:
	
		String buildRequest(String url, String body, String getOrPost = "POST");
		String sendRequest(String reg);
	
		WiFiClientSecure * secure_client;
		uint16_t port;
		
		
		String api_token;
		const String push_bullet_host = "api.pushbullet.com";
		
	
	
};


#endif
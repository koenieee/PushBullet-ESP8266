#include <PushBullet.h>


PushBullet::PushBullet(const String api_token, WiFiClientSecure * secure_client, uint16_t port){
	this->secure_client = secure_client;
	this->api_token = api_token;
	this->port = port;
	
}

bool PushBullet::openConnection(){
  if (!secure_client->connect(push_bullet_host.c_str(), port)) {
    return false;
  }
   return true;
}

bool PushBullet::closeConnection(){
	secure_client->stop();
	if(secure_client->connected() == 1){
		return false;
	}
	else{
		return true;
	}
}

bool PushBullet::checkConnection(){
	
	
}

String PushBullet::buildRequest(String url, String body){
	String request = String("POST ") + url +" HTTP/1.1\r\n" +
                   "Host: " + push_bullet_host + "\r\n" +
                   "User-Agent: ESP8266/NodeMCU 0.9\r\n" +
                   "Accept: */*\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: "+body.length()+"\r\n" +
                   "Access-Token: "+ api_token +"\r\n\r\n" +
                   body;
	return request;
}

void PushBullet::sendNotePush(const String message, const String title){
	String req = buildRequest("/v2/pushes", "{\"body\":\""+message+"\",\"title\":\""+title+"\",\"type\":\"note\"}");
				   
#ifdef DEBUGGING
    Serial.println("Request string: ");
	Serial.println(req);
#endif
	
				   
	secure_client->print(req);
}

void PushBullet::sendLinkPush(const String message, const String title, const String url){
	String req = buildRequest("/v2/pushes", "{\"body\":\""+message+"\",\"title\":\""+title+"\",\"url\":\""+url+"\",\"type\":\"link\"}");
				   
#ifdef DEBUGGING
    Serial.println("Request string: ");
	Serial.println(req);
#endif
	
				   
	secure_client->print(req);
	
}

void PushBullet::sendSMSPush(const String message, const String phoneNumber, const String source_device, const String source_user){
	String req = buildRequest("/v2/ephemerals", "{ \"push\": {    \"conversation_iden\": \""+phoneNumber+"\",    \"message\": \""+message+"\",    \"package_name\": \"com.pushbullet.android\",    \"source_user_iden\": \""+source_user+"\",    \"target_device_iden\": \""+source_device+"\",    \"type\": \"messaging_extension_reply\"  },  \"type\": \"push\"}			");
	
#ifdef DEBUGGING
    Serial.println("Request string: ");
	Serial.println(req);
#endif
	
				   
	secure_client->print(req);
	
}

void PushBullet::copyToClipboard(const String contents, const String source_device, const String source_user){
 	String req = buildRequest("/v2/ephemerals", "{\"push\":{\"body\":\""+contents+"\",\"source_device_iden\":\""+source_device+"\",\"source_user_iden\":\""+source_user+"\",\"type\":\"clip\"},\"type\":\"push\"}");
	
#ifdef DEBUGGING
    Serial.println("Request string: ");
	Serial.println(req);
#endif
	
				   
	secure_client->print(req);
}
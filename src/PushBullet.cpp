#include <PushBullet.h>


PushBullet::PushBullet(const String api_token, WiFiClientSecure * secure_client, uint16_t port){
	this->secure_client = secure_client;
	this->api_token = api_token;
	this->port = port;
	
}

bool PushBullet::openConnection(){
  if (!this->secure_client->connect(this->push_bullet_host.c_str(), this->port)) {
    return false;
  }
   return true;
}

bool PushBullet::closeConnection(){
	this->secure_client->stop();
	return checkConnection();
}

bool PushBullet::checkConnection(){
	if(this->secure_client->connected() == 1){
		return true;
	}
	else{
		return false;
	}	
}

void PushBullet::togglConnection(){
	if(checkConnection()){
		closeConnection();
	}
	else{
		openConnection();
	}	
}

String PushBullet::buildRequest(String url, String body){
	String request = String("POST ") + url +" HTTP/1.1\r\n" +
                   "Host: " + this->push_bullet_host + "\r\n" +
                   "User-Agent: ESP8266/NodeMCU 0.9\r\n" +
                   "Accept: */*\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: "+ body.length() +"\r\n" +
                   "Access-Token: "+ this->api_token +"\r\n\r\n" +
                   body;
	return request;
}

bool PushBullet::sendRequest(String req){
if(
#ifdef DEBUGGING
    Serial.println("Request string: ");
	Serial.println(req);
#endif

	this->secure_client->print(req);
	
}

void PushBullet::sendAndToggl(String req){
	togglConnection();
	sendRequest(req);
	togglConnection();
}
	

void PushBullet::sendNotePush(const String message, const String title){
	sendAndToggl("/v2/pushes", "{\"body\":\""+message+"\",\"title\":\""+title+"\",\"type\":\"note\"}");
}

void PushBullet::sendLinkPush(const String message, const String title, const String url){
	sendAndToggl("/v2/pushes", "{\"body\":\""+message+"\",\"title\":\""+title+"\",\"url\":\""+url+"\",\"type\":\"link\"}");		   
}

void PushBullet::sendSMSPush(const String message, const String phoneNumber, const String source_device, const String source_user){
	sendAndToggl("/v2/ephemerals", "{ \"push\": {    \"conversation_iden\": \""+phoneNumber+"\",    \"message\": \""+message+"\",    \"package_name\": \"com.pushbullet.android\",    \"source_user_iden\": \""+source_user+"\",    \"target_device_iden\": \""+source_device+"\",    \"type\": \"messaging_extension_reply\"  },  \"type\": \"push\"}			");
}

bool PushBullet::copyToClipboard(const String contents, const String source_device, const String source_user){
	sendAndToggl("/v2/ephemerals", "{\"push\":{\"body\":\""+contents+"\",\"source_device_iden\":\""+source_device+"\",\"source_user_iden\":\""+source_user+"\",\"type\":\"clip\"},\"type\":\"push\"}");
}

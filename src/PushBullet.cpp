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
                   "User-Agent: curl/7.44.0\r\n" +
                   "Accept: */*\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: "+body.length()+"\r\n" +
                   "Access-Token: "+ api_token +"\r\n\r\n" +
                   body;
	return request;
}

void PushBullet::sendNormalPush(const String message, const String title, PUSH_TYPE theType){
	String type;
	switch(theType){
		case NOTE: 
			type = "note";
		break;
	}
	
	String req = buildRequest("/v2/pushes", "{\"body\":\""+message+"\",\"title\":\""+title+"\",\"type\":\""+type+"\"}");
				   
	Serial.println("Request string: ");
	Serial.println(req);
				   
	secure_client->print(req);
}

void PushBullet::sendSMSPush(){
	
	
}

void PushBullet::copyToClipboard(){
	
	
}
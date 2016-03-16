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
	if(this->secure_client->connected() == 1){
		return false;
	}
	else{
		return true;
	}
}

bool PushBullet::checkConnection(){
	
	
}

String PushBullet::buildRequest(String url, String body, String getOrPost){
	String request = getOrPost +" "+ url +" HTTP/1.1\r\n" +
                   "Host: " + this->push_bullet_host + "\r\n" +
                   "User-Agent: ESP8266/NodeMCU 0.9\r\n" +
                   "Accept: */*\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: "+ body.length() +"\r\n" +
                   "Access-Token: "+ this->api_token +"\r\n\r\n" +
                   body;
	return request;
}

String PushBullet::sendRequest(String req){

#ifdef DEBUGGING
    Serial.println("Request string: ");
	Serial.println(req);
#endif

	
	//if (this->secure_client->available()) {
	this->secure_client->print(req);
	delay(30);

#ifdef RETURN_FULLHEADER

	return this->secure_client->readString();
	
#else
	
	unsigned int i = 0; //timeout counter
	int n = 1; // char counter
	char json[600] = "{";

    while (!this->secure_client->find("\"pushes\":[{")){} 
	while (i<600) {
		char c = this->secure_client->read();
		json[n]=c;
		if(c=='}') break;
		n++;
		i=0;
		i++;
	}
	return json;

#endif

	
}


String PushBullet::sendNotePush(const String message, const String title){
	String req = buildRequest("/v2/pushes", "{\"body\":\""+message+"\",\"title\":\""+title+"\",\"type\":\"note\"}");
	return sendRequest(req);

}

String PushBullet::sendLinkPush(const String message, const String title, const String url){
	String req = buildRequest("/v2/pushes", "{\"body\":\""+message+"\",\"title\":\""+title+"\",\"url\":\""+url+"\",\"type\":\"link\"}");		   
	return sendRequest(req);
}

String PushBullet::sendSMSPush(const String message, const String phoneNumber, const String source_device, const String source_user){
	String req = buildRequest("/v2/ephemerals", "{ \"push\": {    \"conversation_iden\": \""+phoneNumber+"\",    \"message\": \""+message+"\",    \"package_name\": \"com.pushbullet.android\",    \"source_user_iden\": \""+source_user+"\",    \"target_device_iden\": \""+source_device+"\",    \"type\": \"messaging_extension_reply\"  },  \"type\": \"push\"}			");
	return sendRequest(req);
	
}

String PushBullet::copyToClipboard(const String contents, const String source_device, const String source_user){
 	String req = buildRequest("/v2/ephemerals", "{\"push\":{\"body\":\""+contents+"\",\"source_device_iden\":\""+source_device+"\",\"source_user_iden\":\""+source_user+"\",\"type\":\"clip\"},\"type\":\"push\"}");
	return sendRequest(req);
}

String PushBullet::registerThisDevice(const String nickName, const String pushToken = ""){
	String req = buildRequest("/v2/devices", "{\"app_version\":8623,\"manufacturer\":\"ESP\",\"model\":\"ESP8266\",\"nickname\":\""+nickName+"\"}");//,\"push_token\":\"production:"+pushToken+"\"
	return sendRequest(req);
}

String PushBullet::getLatestPushed(const String modified_after, int limit){
	String req = buildRequest("/v2/pushes?active=true&modified_after="+modified_after+"&limit="+limit+"", "", "GET");
	return sendRequest(req);

}

pushesJsonObject PushBullet::convertJsonToStruct(char json[600]){
	pushesJsonObject jo;

	Serial.println("JSON DATA: ");
	Serial.println(json);
	StaticJsonBuffer<2400> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);

	if (!root.success()) {
		jo.message = "";
		jo.title = "";
		jo.byWho = "";
		return jo;
	}
	
	jo.message = root["body"].asString();  
	jo.title = root["title"].asString();  
	jo.byWho = root["sender_name"].asString(); 

	return jo;
	

	
}



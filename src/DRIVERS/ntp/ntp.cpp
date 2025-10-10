void ntc_init(const char* ssid, const char* password){
    int connection_tries_counter = 0;

    /*Connect to Wi-Fi*/
    WiFi.begin(ssid, password);

    while (connection_tries_counter < MAX_RETRY_NUMBER){
        if(WiFi.status() != WL_CONNECTED){
            sleep(1000);
            connection_tries_counter += 1;
        }

    if(counter >= 5){}
  }
}
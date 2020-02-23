
#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>
 

String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
 
String html_1 = R"=====(
<!DOCTYPE html>
<html>
 <head>
 <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
 <meta charset='utf-8'>
 <style>
  body {font-size:140%;} 
  #main {display: table; margin: auto;  padding: 0 10px 0 10px; } 
  h2 {text-align:center; } 
  #LED_button1 { padding:10px 10px 10px 10px; width:100%;  background-color: #50FF50; font-size: 120%;}
    #LED_button2 { padding:10px 10px 10px 10px; width:100%;  background-color: #f11000; font-size: 120%;}
 </style>
 <title>LED Control</title>
</head>
<body>
 <div id='main'>
  <h2>LED Control</h2>
  
  <input type="button" id = "LED_button1" onclick="switchLED1()" value="Turn on the LED1"  /> 

  <input type="button" id = "LED_button2" onclick="switchLED2()" value="Turn on the LED2"  /> 

   
 </div>
 <script>
    function switchLED1() 
  {
       var button_text = document.getElementById("LED_button1").value;
     if (button_text=="Turn on the LED1")
     {
       document.getElementById("LED_button1").value = "Turning on the LED1";
       ajaxLoad1('LEDON1'); 
     }
     else
     {
       document.getElementById("LED_button1").value = "Turning off the LED1";
       ajaxLoad1('LEDOFF1');
     }
    }
    
    function switchLED2() 
  {
       var button_text = document.getElementById("LED_button2").value;
     if (button_text=="Turn on the LED2")
     {
       document.getElementById("LED_button2").value = "Turning on the LED2";
       ajaxLoad2('LEDON2'); 
     }
     else
     {
       document.getElementById("LED_button2").value = "Turning off the LED2";
       ajaxLoad2('LEDOFF2');
     }
    }
    
 
var ajaxRequest = null;

if (window.XMLHttpRequest)  { ajaxRequest =new XMLHttpRequest(); }
else                        { ajaxRequest =new ActiveXObject("Microsoft.XMLHTTP"); }
 
 
function ajaxLoad1(ajaxURL)
{
  if(!ajaxRequest){ alert("AJAX is not supported."); return; }
 
  ajaxRequest.open("GET",ajaxURL,true);
  ajaxRequest.onreadystatechange = function()
  {
    if(ajaxRequest.readyState >0 && ajaxRequest.status==200)
    {
      var ajaxResult = ajaxRequest.responseText;
      if      ( ajaxResult == "LED is on1" )   { document.getElementById("LED_button1").value = "Turn off the LED1"; }
      else if ( ajaxResult == "LED is off1" )  { document.getElementById("LED_button1").value = "Turn on the LED1"; }

    }
  }
  ajaxRequest.send();
}


 function ajaxLoad2(ajaxURL)
{
  if(!ajaxRequest){ alert("AJAX is not supported."); return; }
 
  ajaxRequest.open("GET",ajaxURL,true);
  ajaxRequest.onreadystatechange = function()
  {
    if(ajaxRequest.readyState >0 && ajaxRequest.status==200)
    {
      var ajaxResult = ajaxRequest.responseText;
      if      ( ajaxResult == "LED is on2" )   { document.getElementById("LED_button2").value = "Turn off the LED2"; }
      else if ( ajaxResult == "LED is off2" )  { document.getElementById("LED_button2").value = "Turn on the LED2"; }

    }
  }
  ajaxRequest.send();
}
</script>
</body>
</html>
 
)=====";

 

 IPAddress    apIP(42, 42, 42, 42);  
                                    
const char *ssid = "WEDEX";
//const char *password = "12345678";

WiFiServer server(80);
 
 
String request = "";
int LED_Pin1 = D3;
 int LED_Pin2 = D4;
 
void setup() 
{
    pinMode(LED_Pin1, OUTPUT);     
 pinMode(LED_Pin2, OUTPUT);     
  // Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   
  WiFi.softAP(ssid
  //, password
  );

 
    
    server.begin();

 
} 
 
 
 
void loop() 
{
    
    WiFiClient client = server.available();
    if (!client)  {  return;  }

    request = client.readStringUntil('\r');
 
    Serial.print("request: "); Serial.println(request); 
 
    if       ( request.indexOf("LEDON1") > 0 )  
             { 
                digitalWrite(LED_Pin1, HIGH);  
                client.print( header );
                client.print( "LED is on1" );             
             }
               if       ( request.indexOf("LEDON2") > 0 )  
             { 
                digitalWrite(LED_Pin2, HIGH);  
                client.print( header );
                client.print( "LED is on2" );             
             }
    else if  ( request.indexOf("LEDOFF1") > 0 ) 
             { 
                digitalWrite(LED_Pin1, LOW);   
                client.print( header );
                client.print( "LED is off1" ); 
             }

 
    else if  ( request.indexOf("LEDOFF2") > 0 ) 
             { 
                digitalWrite(LED_Pin2, LOW);   
                client.print( header );
                client.print( "LED is off2" ); 
             }
    else
    {
        client.flush();
        client.print( header );
        client.print( html_1 );    
        delay(5);
    }
} 

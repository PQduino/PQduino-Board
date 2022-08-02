#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <flash_hal.h>
#include <FS.h>
#include "StreamString.h"
#include "ESP8266HTTPUpdateServer.h"

namespace esp8266httpupdateserver {
using namespace esp8266webserver;

static const char serverIndex[] PROGMEM =
R"(
  <html lang='en'>
  <head>
    <meta charset=utf-8>
      <title>ESP8266 Powermeter Firmware Update</title>
    </head>
    <body>
  <style>
    html, body {
        font-family: Trebuchet MS, sans-serif;
        font-size: 16px;
        color: #3399FF;
        width: 100%;
        height:100%;
        background: linear-gradient(0deg,#1e2730,#1e2730,#1e2730, #717171);
        background-size: 400% 400%;
        display: table;
        position: relative;
    }
    .lines {
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      height: 10%;
      margin: auto;
      width: 100%;
    }
    .line {
      position: absolute;
      width: 110%;
      height: 1px;
      top: 0;
      left: -5%;
      background: rgba(255, 255, 255, 0.1);
      overflow: hidden;
    }
    .line::after {
      content: '';
      display: block;
      position: absolute;
      height: 100%;
      width: 15vh;
      top: 0%;
      left: 0;
      background: linear-gradient(to right, rgba(255, 255, 255, 0) 0%, #ffffff 75%, #ffffff 100%);

        animation: drop 7s 0s infinite;
        animation-fill-mode: forwards;
        animation-timing-function: cubic-bezier(0.4, 0.26, 0, 0.97);
    }
    @keyframes drop {0% {left: -50%;} 100% {left: 110%;}}
    .button-send {
        width: 100%;
        display: inline-block;
        padding: 5px 55px;
        font-size: 16px;
        cursor: pointer;
        text-align: center;
        color: #fff;
        background-color: #337ab7;
        border: none;
        border-radius: 10px;
    }
    .button-send:hover {
        background-color: #2e6da4
    }
    .button-send:active {
        background-color: #337ab7;
        box-shadow: 1px 1px #666;
        transform: translateY(4px);
    }
    .button-send:disabled {
        background-color: #9c9c9c;
        box-shadow: 1px 1px #666;
    }
    .container {
        padding-top: 80px;
        display: flex;
        justify-content: center;
    }
    .form-container {
        min-width: 380px;
    }
    .progress {
        width: 100%;
        height: 8px;
        background: #e1e4e8;
        border-radius: 4px;
        overflow: hidden;
    }
    .progress .progress-bar {
        display: block;
        height: 100%;
        background: linear-gradient(90deg, #ffd33d, #ea4aaa 17%, #b34bff 34%, #01feff 51%, #ffd33d 68%, #ea4aaa 85%, #b34bff);
        background-size: 300% 100%;
        -webkit-animation: progress-animation 2s linear infinite;
        animation: progress-animation 2s linear infinite;
    }
    .additional-info {
        height: 67px;
    }
    @-webkit-keyframes progress-animation {
        0% {
            background-position: 100%;
        }
        100% {
            background-position: 0;
        }
    }
    @keyframes progress-animation {
        0% {
            background-position: 100%;
        }
        100% {
            background-position: 0;
        }
    }
    .tile-container {
        display: flex;
        justify-content: center;
    }
    .button-o {
        cursor: pointer;
        height: 25px;
        font-size: 15px;
        background: none;
        outline: none;
        border: 1px solid rgba(0, 0, 0, 0.35);
        width: 50%;
        background: rgba(51, 122, 183, 0.3);
    }
    .button-o.selected {
        background-color: #337ab7;
        color: white;
    }
    .button-o.left {
        border-radius: 7px 0px 0px 7px;
        margin-right: -3px;
    }
    .button-o.right {
        border-radius: 0px 7px 7px 0px;
        margin-left: -3px;
    }
    #overlay {
        position: fixed; /* Sit on top of the page content */
        display: none; /* Hidden by default */
        width: 100%; /* Full width (cover the whole page) */
        height: 100%; /* Full height (cover the whole page) */
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: rgba(0, 0, 0, 0.06); /* Black background with opacity */
        z-index: 2;
        cursor: pointer; /* Add a pointer on hover */
    }
    .container-input {
        width: 100%;
        height: 35px;
        border: 0px transparent;
        text-align: center;
        line-height: 35px;
        margin-top: 20px;
        margin-bottom: 20px;
        background-color: rgba(51, 122, 183, 0.3);
        border-radius: 7px;
    }
    .inputfile {
        width: 0.1px;
        height: 0.1px;
        opacity: 0;
        overflow: hidden;
        position: absolute;
        z-index: -1;
    }
    .inputfile + label {
        max-width: 100%;
        height: 35px;
        font-size: 1rem;
        font-weight: 700;
        text-overflow: ellipsis;
        white-space: nowrap;
        cursor: pointer;
        display: inline-block;
        border-radius: 7px;
    }
    .newinputfile {
      font-size:16px; 
      font-weight:normal;
    }
    .inputfile-1 + label {
        color: #fff;                        
    }
    .inputfile-1:focus + label,
    .inputfile-1.has-focus + label,
    .inputfile-1 + label:hover {
        background-color: #337ab7;
        width: 100%;
        font-size:6px;
    }
   .box-icons {
    width: 100%;
    display: flex;
    justify-content: center;
    padding-top: 10px;
    padding-left: 20px;
   }
   .home_bt {
    width:120px;
    height:32px;
    float: center;
    color: #696969;
    cursor: pointer;
    text-align: center;
    border: 0px transparent;
    background:
    url('data:image/png;base64,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')
    no-repeat
    left center;
    padding: 0px 0px 0px 0px;
    background-size: 32px;
  }
  .home_bt:hover {
    color: #337ab7;
  }
</style>
<div class='lines'> 
  <div class='line'></div>
</div>
<div id="overlay"></div>
<div class="tile-container">
    <h1>Firmware Update</h1>
</div>
<div class='container'>
    <form enctype="multipart/form-data" class="form-container" id="upload_form" method="post">
        <div id="switch-container">
            <button class="button-o left selected" id="firmware-button" type="button">Firmware</button>
            <button class="button-o right" id="filesystem-button" type="button">FileSystem</button>
        </div>
        <div class="container-input">
        <input accept='.bin,.bin.gz' input type="file" name="firmware" id="file1" class="inputfile inputfile-1"/>
        <label for="file1">
        <span class="newinputfile">Select File</span>
        </label>
        </div>
        <div class="progress">
            <span class="progress-bar" id="progressBar" style="width: 0%"></span>
        </div>
        <div class="additional-info">
            <h3 id="status"></h3>
            <p id="loaded_n_total"></p>
        </div>
        <hr/>
        <button id="button-send" class="button-send" type="submit" disabled>Upload
        </button>
    </form>
</div>
<div class='box-icons'>
  <form  class='form-icons' action='/'>
      <button class='home_bt' href='/' type='submit'>
          <img class='home_icon'/>Home<br>Page
      </button>
  </form>
</div>
<script type="application/javascript">
    'use strict';
;( function ( document, window, index )
{
  var inputs = document.querySelectorAll( '.inputfile' );
  Array.prototype.forEach.call( inputs, function( input )
  {
    var label  = input.nextElementSibling,
      labelVal = label.innerHTML;
    input.addEventListener( 'change', function( e )
    {
      var fileName = '';
      if( this.files && this.files.length > 1 )
        fileName = ( this.getAttribute( 'data-multiple-caption' ) || '' ).replace( '{count}', this.files.length );
      else
        fileName = e.target.value.split( '\\' ).pop();
      if( fileName )
        label.querySelector( 'span' ).innerHTML = fileName;
      else
        label.innerHTML = labelVal;
    });
  });
}( document, window, 0 ));
        function stringToBoolean(string){
            switch(string.toLowerCase().trim()){
                case "true": case "yes": case "1": return true;
                case "false": case "no": case "0": case null: return false;
                default: return Boolean(string);
            }
        }
        const urlParams = new URLSearchParams(window.location.search);
        const onlyFirmware = urlParams.get('onlyFirmware');
        if (onlyFirmware && stringToBoolean(onlyFirmware)===true){
            _('switch-container').style.display = 'none';
        }
        function disableAll() {
            document.getElementById("overlay").style.display = "block";
        }
        function enableAll() {
            document.getElementById("overlay").style.display = "none";
        }
        function _(el) {
            return document.getElementById(el);
        }
        function uploadFile() {
            var file = _("file1").files[0];
            // alert(file.name+" | "+file.size+" | "+file.type);
            var formdata = new FormData();
            formdata.append(_("file1").name, file, file.name);
            var ajax = new XMLHttpRequest();
            ajax.upload.addEventListener('progress', progressHandler, false);
            ajax.addEventListener('load', completeHandler, false);
            ajax.addEventListener('loadstart', startHandler, false);
            ajax.addEventListener('error', errorHandler, false);
            ajax.addEventListener('abort', abortHandler, false);
            ajax.open("POST", "/firmware");
            ajax.setRequestHeader('Access-Control-Allow-Headers', '*');
            ajax.setRequestHeader('Access-Control-Allow-Origin', '*');

            ajax.send(formdata);
        }
        function progressHandler(event) {
            _('loaded_n_total').innerHTML = "Uploaded " + event.loaded + " bytes of " + event.total;
            var percent = Math.round((event.loaded / event.total) * 100);
            _('progressBar').style = 'width: ' + percent + '%';
            _('status').innerHTML = percent + "% uploaded... please wait";
        }
        function completeHandler(event) {
            enableAll();
            if (event.target.responseText.indexOf('error')>=0){
                _('status').innerHTML = event.target.responseText;
            }else {
                _('status').innerHTML = 'Upload Success!'+urlParams; //event.target.responseText;
            }
            _('progressBar').value = 0; //wil clear progress bar after successful upload
        }
        function startHandler(event) {
            disableAll();
        }
        function errorHandler(event) {
            enableAll();
            _('status').innerHTML = "Upload Failed";
        }
        function abortHandler(event) {
            enableAll();
            _('status').innerHTML = "Upload Aborted";
        }
        _('upload_form').addEventListener('submit', (e) => {
            uploadFile();
        });
        _('firmware-button').addEventListener('click',
            function (e) {
                e.target.classList.add('selected');
                _('filesystem-button').classList.remove('selected');
                _("file1").name = 'firmware';
            }
        )
        _('filesystem-button').addEventListener('click',
            function (e) {
                e.target.classList.add('selected');
                _('firmware-button').classList.remove('selected');
                _("file1").name = 'filesystem';
            }
        )
        _('file1').addEventListener('change', function(e){
            var file = _("file1").files[0];
            if (file && file.name){
                _('button-send').disabled = false;
                _('status').innerHTML = "You can upload firmware!";
            }else{
                _('button-send').disabled = true;
            _('status').innerHTML ="" ;
            }
            _('loaded_n_total').innerHTML = "";
        });
    </script>
</body>
</html>
)";
static const char successResponse[] PROGMEM = 
R"(
  <!DOCTYPE html>
  <html lang='en'>
  <head>
      <meta http-equiv="refresh" content="20; URL=/">
      <title>ESP8266 Powermeter Firmware Update</title>
    </head>
    <body>
  <style>
    html, body {
        font-family: Trebuchet MS, sans-serif;
        font-size: 16px;
        color: #3399FF;
        width: 100%;
        height:100%;
        background: linear-gradient(0deg,#1e2730,#1e2730,#1e2730, #717171);
        background-size: 400% 400%;
        display: table;
        position: relative;
    }
    .lines {
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      height: 10%;
      margin: auto;
      width: 100%;
    }
    .line {
      position: absolute;
      width: 110%;
      height: 1px;
      top: 0;
      left: -5%;
      background: rgba(255, 255, 255, 0.1);
      overflow: hidden;
    }
    .line::after {
      content: '';
      display: block;
      position: absolute;
      height: 100%;
      width: 15vh;
      top: 0%;
      left: 0;
      background: linear-gradient(to right, rgba(255, 255, 255, 0) 0%, #ffffff 75%, #ffffff 100%);
      animation: drop 7s 0s infinite;
      animation-fill-mode: forwards;
      animation-timing-function: cubic-bezier(0.4, 0.26, 0, 0.97);
    }
    @keyframes drop {0% {left: -50%;} 100% {left: 110%;}}
    .button-send {
        width: 100%;
        display: inline-block;
        padding: 5px 55px;
        font-size: 16px;
        cursor: pointer;
        text-align: center;
        color: #fff;
        background-color: #337ab7;
        border: none;
        border-radius: 10px;
    }
    .container {
        padding-top: 80px;
        display: flex;
        justify-content: center;
    }
    .form-container {
        min-width: 380px;
    }
    .additional-info {
        height: 67px;
    }
</style>
<div class='lines'> 
  <div class='line'></div>
</div>
<div class='container'>
        <div class="additional-info">
            <h3 id="status">Update Success! Rebooting...please wait</h3>
            <p id="loaded_n_total"></p>
        </div>
</div>
<script>
</script>
</body>
</html>
)";

template <typename ServerType>
ESP8266HTTPUpdateServerTemplate<ServerType>::ESP8266HTTPUpdateServerTemplate(bool serial_debug)
{
  _serial_output = serial_debug;
  _server = NULL;
  _username = emptyString;
  _password = emptyString;
  _authenticated = false;
}

template <typename ServerType>
void ESP8266HTTPUpdateServerTemplate<ServerType>::setup(ESP8266WebServerTemplate<ServerType> *server, const String& path, const String& username, const String& password)
{
    _server = server;
    _username = username;
    _password = password;

    // handler for the /update form page
    _server->on(path.c_str(), HTTP_GET, [&](){
      if(_username != emptyString && _password != emptyString && !_server->authenticate(_username.c_str(), _password.c_str()))
        return _server->requestAuthentication();
      _server->send_P(200, PSTR("text/html"), serverIndex);
    });

    // handler for the /update form POST (once file upload finishes)
    _server->on(path.c_str(), HTTP_POST, [&](){
      if(!_authenticated)
        return _server->requestAuthentication();
      if (Update.hasError()) {
        _server->send(200, F("text/html"), String(F("Update error: ")) + _updaterError);
      } else {
        _server->client().setNoDelay(true);
        _server->send_P(200, PSTR("text/html"), successResponse);
        delay(100);
        //for(int i = 1; i<100; i++) {delayMicroseconds(1000);}
        _server->client().stop();
        ESP.restart();
      }
    },[&](){
      // handler for the file upload, gets the sketch bytes, and writes
      // them through the Update object
      HTTPUpload& upload = _server->upload();

      if(upload.status == UPLOAD_FILE_START){
        _updaterError.clear();
        if (_serial_output)
          Serial.setDebugOutput(true);

        _authenticated = (_username == emptyString || _password == emptyString || _server->authenticate(_username.c_str(), _password.c_str()));
        if(!_authenticated){
          if (_serial_output)
            Serial.printf("Unauthenticated Update\n");
          return;
        }

        WiFiUDP::stopAll();
        if (_serial_output)
          Serial.printf("Update: %s\n", upload.filename.c_str());
        if (upload.name == "filesystem") {
          size_t fsSize = ((size_t) &_FS_end - (size_t) &_FS_start);
          close_all_fs();
          if (!Update.begin(fsSize, U_FS)){//start with max available size
            if (_serial_output) Update.printError(Serial);
          }
        } else {
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if (!Update.begin(maxSketchSpace, U_FLASH)){//start with max available size
            _setUpdaterError();
          }
        }
      } else if(_authenticated && upload.status == UPLOAD_FILE_WRITE && !_updaterError.length()){
        if (_serial_output) Serial.printf(".");
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          _setUpdaterError();
        }
      } else if(_authenticated && upload.status == UPLOAD_FILE_END && !_updaterError.length()){
        if(Update.end(true)){ //true to set the size to the current progress
          if (_serial_output) Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          _setUpdaterError();
        }
        if (_serial_output) Serial.setDebugOutput(false);
      } else if(_authenticated && upload.status == UPLOAD_FILE_ABORTED){
        Update.end();
        if (_serial_output) Serial.println("Update was aborted");
      }
      delay(0);
    });
}

template <typename ServerType>
void ESP8266HTTPUpdateServerTemplate<ServerType>::_setUpdaterError()
{
  if (_serial_output) Update.printError(Serial);
  StreamString str;
  Update.printError(str);
  _updaterError = str.c_str();
}

};

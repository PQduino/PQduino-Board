//=====================
//HTML code for webpage
//=====================
const char main_webpage[] PROGMEM = 
R"=====(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset=utf-8>
      <title>ESP8266 Powermeter</title>
    </head>
    <body>
<style>
    html, body {
        font-family: Trebuchet MS, sans-serif;
        font-size: 16px;
        text-align: center;
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
    canvas{border-radius: 15px;}
    .box{
          display: flex;
          justify-content: center;
          max-width: 100%;
          padding: 130px 0px 0px 0px;
    }
    .box-graph{
          max-width: 205px;
          min-width: 205px;
          float: left;
          padding: 5px 5px 5px 5px;
          max-height: 455px;
          min-height: 455px;
          text-align: center;
          display: inline-block;
          margin: 10px 10px;
          background-color: #fff;
          box-shadow: 0 27px 87px rgba(0,0,0,0.5);
          border-radius: 15px;
          overflow: hidden;
    }
    .box-text{
          width: 50%;
          float: left;
          padding: 5px 35px 5px 35px;
          max-height: 455px;
          min-height: 455px;
          display: inline-block;
          margin: 10px 10px;
          background-color: #fff;
          box-shadow: 0 27px 87px rgba(0,0,0,0.5);
          border-radius: 15px;
          overflow: hidden;
    }
    .box-graph p{
        font-size: 14px;
        font-weight: bold;
        color: #696969;
        line-height: 5px;
        letter-spacing: 0px;
    }
    .box-text h1{
        font-size: 25px;
        font-weight: bold;
        text-align: left;
        color: #696969;
        line-height: 15px;
        letter-spacing: 0.1px;
    }
    .box-text p{
        font-size: 14px;
        font-weight: 400;
        text-align: left;
        color: #696969;
        line-height: 3.5px;
        letter-spacing: 0px;
    }
    .box-icons{
          width: calc(50% + 305px);
          justify-content: right;
          border: 0px transparent;
          display: inline-block;
    }
    .debug-box{
          max-width: calc(50% + 305px);
          text-align: left;
          display: inline-block;
          background-color:#696969;
    }
    .logo {
        position: absolute;
        top: 0px;
        left: calc(50% - 90px);
        padding: 5px 0px 0px 0px;
    }
    .title{
        position: absolute;
        width: 326px;
        top: 0px;
        left: calc(50% - 163px);
        padding: 70px 0px 0px 0px;
    }
    .setup_bt {
      width:90px;
      height:32px;
      float: right;
      color: #696969;
      cursor: pointer;
      text-align: right;
      border: 0px transparent;
      background:
      url('data:image/png;base64,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')
      no-repeat
      left center;
      padding: 0px 10px 0px 0px;
      background-size: 32px;
    }
    .firmware_bt {
      width:100px;
      height:32px;
      float: right;
      color: #696969;
      cursor: pointer;
      text-align: right;
      border: 0px transparent;
      background:
      url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA7AAAAOwBeShxvQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAXqSURBVFiFrVdrbFRVEP6+s2fX7YNKf1CkiBpeAgrxSSPUWExESQQEUcBAAopFeShIcJEgYCDQCibyEhGSRiUUjQKCBDEIRikIkRAN4RUQEKRACeVVHt17z/jj3r17b3sXhTDJJrdn5sx8Z853ZqbEDSR37Z9PibArKOpGdhlFaAjZcblvl18ymTCToumqPeMhGEXhGlBszyf4oEBaK2Ct394AfQj+RcgeH4CIUF6AyMLzAzrP+08Azb7am/D9OZFiF50Z1Pmw3+auyn332Up+U1RPnH65wxEAaP7twdbGsrZpjaLqFzsdC/j8Zm872NgGYG5qrWZgp3IPQOGKA8UQuzsAiLBMhOUAQEqiekjH0AwVfrl/uFBmA1gDAUH0FSBxakjHz8PsWyzfJ36/pExyokeqtE4yDqPznQUBKLVO+jJf+8mhHSru/WL/zyKqJwAoY88+Orzj0Uz2UUsF/QqdeArxwAnbLjtYlvoWIHF4RPuMHLkZabPsoBDw0n5oRPtJqW8dRBqpTQOQ2xHb9atAsDZMpx9YdLhYCRwOWCgTFykzP5CbFm0RAuQ7fpHovNDhtSGqdKwecbhKgABdpHIbM5Bs6FfceIjr3ePbbAKwCQC6zjmCNJhwEnabc6jAFj1WiOcBtHOXD1Kwvt5Kzt89uV1NIwCW8gVl+c6JrT0OBKLELFWb+kWtxlfQbdaxAbSjB7TNu7WlSu26WIFdFyuImMgb2marbMYOdJ91tH9jAAz49ut0ycxjxRDVHQBgoQw+tvqlZMaJAbBlBQy2Xmp2unTXyMeSPvXOR5f8vrtJTfN7IFjZY8bxQVveb7UqmIFUZpEomXHcTYap0trSaQ6IgOK8V2E6Az2nnyowlrUEjDwNsSfkV7dY98zU41v8AKVa9YCYC46NWd3rvepfN8xuUQM4JAz4JVPXHNebprf0ONBr8j8AUkpfmpJmrAizATMT4BUIC0EVtBMWgjzh2uRCmTEApgEuCdN+yzfMahleB7TvfvwM0En0FnAwVOQibfMWyFVrPyyc7t/b593qaxD1kETUXBiTR3CqB8BSECC8Drw04UyxpHqBxTKALgfSzzBqqdagPExLrghVDkOeaMwChCqHtjwuZDaEbdP7/RmQxIB3qp1DMlKlWS9xqohTJASgoBEHtEsiIe4gkBUGQFsKQmYJkE9Btv8E2lIBv0KXc0bi+usFzT0ODBl9BvAImQYQtXgYws0UXDRKOoRVSe2k+awy/FGIPFC6BzJAr/iUL19UkIkDOs0B3ym1pdYRqARkB4y6AqD/sJHnrvr30pL+AE4AMhVgEb2rdDnADL2g9NVzxTRuL6hHmdDtBb4MMBlbEEVyFMgpACZQcNIQgVeghCeFuA5wFgSrEdULPQDJdNopSIwcdg4AIApVOmKpuILJBwARQsRBSqYzUFHRpGbU0NpSAJsBbLWymj679DP6CxFKSyWqr57fSOAnIxi4uCLvbDqDQb90r8MYFZwH3h500ZsHQEnMq7wzoB/3Sm1/EbUMgu+UwuK6S3V7ACArJ6cziTcF6C3CEfNX5q0O+B18QSDpK5m3Mi+8F0QNar1fSC/4eEX+Kg22j9n4O5JUi/Piuafz4rmnorb6JGrhmAbvbxgccEjo9+3X6US/i8WE0wuYRFlqcsnUjD+qzDsLp8BMy2DSSJyBxOGAAIlJ/S7D+TZVOqtexY1SXi+AW7PB2zeQRJMM+nV7gTKI6+nrc7068MGzV5CqWGHxZz53tVh87ztMKKya8kPWVv+aS0KvDkzbmB1eB2ImPROGTUQRi/GwZtVA4g0XnKk4Qx2YW5IsFpgQDjROQdSKXAPwvaM3gUwQqioToqjFAAfmlFz39uiYhbiBSg2MoNu1wkgYs9KnS+1JiQo5eRqACvhNDagKDf4vWNg16asDfJ0qUjR6Ow9lcvx/5NMiaWfD3i4iy1JrY3ZGPQ5kpPrSR2ScUEYTUilQdbcW3uQCHCzg/JG7OD/M4oZvraKLPAmiBEDOrQFAnQG2vPYHt2Yy+BcN6Io7CadnjQAAAABJRU5ErkJggg==')
      no-repeat
      left center;
      padding: 0px 0px 0px 0px;
      background-size: 32px;
    }
    .firmware_bt:hover {
      color: #337ab7;
    }
    .setup_bt:hover {
      color: #337ab7;
    }
</style>
 <img class="logo" src='data:image/png;base64,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'>
 <h1 class='title'>My Home PowerMeter</h1>
 <div class='box'>
  <div class='box-graph'>
    <canvas id='myRate' width='200' height='180' style='border:1px solid #d3d3d3;'> Your browser does not support the HTML5 canvas tag
        </canvas>
    <p>Energy using rate</p>
    <canvas id='myWave' width='200' height='200' style='border:1px solid #d3d3d3;'> Your browser does not support the HTML5 canvas tag
        </canvas>
    <p id='Type_wave'></p>
  </div>
  <div class='box-text' id='b-t'>
    <h1>Parameters</h1>
    <p>Voltage =<span id='Voltage'>00</span> V
        <p>Current =<span id='Current'>00</span> A
        <p>Frequency =<span id='Frequency'>00</span> Hz
        <p>Active Power =<span id='P_Power'>00</span> W
        <p>Reactive Power =<span id='Q_Power'>00</span> var
        <p>Apparent Power = <span id='S_Power'>00</span> VA
        <p>THD V = <span id='THDV'>00</span> %
        <p>THD I = <span id='THDI'>00</span> %
        <p>Temperature = <span id='Temp'>00</span> °C
        <p>Power Factor =<span id='PF'>00</span> % ind
    </p>
    <div class='lines'>
      <div class='line'></div>
    </div>
    <canvas id='myHysto' width='200' height='200' style='border:1px solid #d3d3d3;'> Your browser does not support the HTML5 canvas tag
        </canvas>
    <p align='center' style='font-weight: bold;'>Consumption History</p>
  </div>
 </div>
 <div class='debug-box' id='debug'>
 </div>
 <div class='box-icons'>
    <form  class='firmware-icons' action='/firmware'>
        <button class='firmware_bt' href='/firmware' type='submit'>
            <img class='firmware_icon'/>Firmware<br>Update
        </button>
    </form>
    <form  class='setup-icons' action='/setup'>
        <button class='setup_bt' href='/setup' type='submit'>
            <img class='firmware_icon'/>System<br>Setup
        </button>
    </form>
</div>
<script>
    var elem = document.getElementById('b-t');
    var properties = window.getComputedStyle(elem, null);
    var d_width = properties.width;
    var h_values=[200];
    var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
    connection.onopen = function () {
        connection.send('Conectado  -  ' + new Date());
        console.log('Conectado  -  ' + new Date());
    }
        connection.onmessage = function (event) {
        console.log('Servidor (recibe): ', event.data);
        console.log(h_values);
        verValor();
    }
        connection.onerror = function (error) {
        console.log('WebSocket Error!!!', error);
    }
    function verValor() {
    var data = event.data;
    var tmp = data.split('#');
    document.getElementById('Voltage').innerHTML = tmp[0];  
    document.getElementById('Current').innerHTML = tmp[1];
    document.getElementById('P_Power').innerHTML = tmp[3];
    document.getElementById('S_Power').innerHTML = tmp[4];
    document.getElementById('Q_Power').innerHTML = tmp[5];
    document.getElementById('THDV').innerHTML = tmp[6];
    document.getElementById('THDI').innerHTML = tmp[7];
    document.getElementById('Temp').innerHTML = tmp[8];
    document.getElementById('PF').innerHTML = tmp[9];
    document.getElementById('Frequency').innerHTML = tmp[16];
    document.getElementById('debug').innerHTML = '';
   
    var energy = tmp[2];
    var h3 = tmp[10];
    var h5 = tmp[11];
    var h7 = tmp[12];
    var h9 = tmp[13];
    var h11 = tmp[14];
    var h13 = tmp[15];

    var thd_sel = tmp[17];
    var thd_ref
    if (thd_sel==1){thd_ref = tmp[6];document.getElementById('Type_wave').innerHTML ='Voltage Waveform';}
    else{thd_ref = tmp[7];document.getElementById('Type_wave').innerHTML ='Current Waveform';}
    var wave_points = tmp[19].split('$');
    var P_val = tmp[3];
    var P_limit = 600;
    h_values.shift();
    h_values[199] = P_val;
    
    var a = document.getElementById('myRate');
    var ctx = a.getContext('2d');
    ctx.clearRect(0, 0, myRate.width, myRate.height); 
    ctx.beginPath();
    myRate.style.backgroundColor = '#1d1f20';
    ctx.lineWidth = 40;
    ctx.strokeStyle = '#EEEEEE';
    ctx.arc(100,105,75,0.75*Math.PI,0.25*Math.PI);
    ctx.stroke();
    ctx.beginPath();
    ctx.strokeStyle = '#00ffff';
    ctx.shadowOffsetX = 0;
    ctx.shadowOffsetY = 5;
    ctx.shadowBlur    = 20;
    if(P_val<=P_limit){
        ctx.shadowColor= '#0000FF';
        ctx.fillStyle = '#3399FF';
        ctx.arc(100,105,75,0.75*Math.PI,(P_val/P_limit*1.5-1.25)*Math.PI);
        ctx.stroke();
    }
    else{
        ctx.shadowColor = '#FF0000';
        ctx.fillStyle = '#FF0000';
        ctx.arc(100,105,75,0.75*Math.PI,(1.5-1.25)*Math.PI);
        ctx.stroke();
    }
    ctx.shadowColor ='rgba(0,0,0,0)';
    ctx.font = 'bold 25px Arial';
    ctx.textAlign = 'center';
    ctx.fillText(P_val+' W',100, 120);
         
    var b = document.getElementById('myWave');
    var ctxw = b.getContext('2d');
    var width = 200;
    var height = 200;
    var scale_y = 0.7;
    var offset = (1 / (wave_points.length - 1)) * width;
    var w_gradient = ctxw.createLinearGradient(0, 0, 170, 0);
    w_gradient.addColorStop('0', '#0ff');
    w_gradient.addColorStop('0.5' ,'#33CCFF');
    w_gradient.addColorStop('1.0', '#3399FF');

    ctxw.strokeStyle = '#1d1f20';
    ctxw.clearRect(0, 0, width, height);
    ctxw.strokeRect(0, 0, width, height);

    ctxw.beginPath();
    ctxw.fill();
    ctxw.strokeStyle = w_gradient;
    myWave.style.backgroundColor = '#1d1f20';
    ctxw.lineWidth = 5;
    ctxw.shadowOffsetX = 0;
    ctxw.shadowOffsetY = 9;
    ctxw.shadowBlur = 20;
    ctxw.shadowColor= '#33CCFF';
    ctxw.moveTo(0, 0.5*height-scale_y*wave_points[0]);
    for (var x = 0; x < wave_points.length; x++) {
        ctxw.lineTo(x * offset, 0.5*height-scale_y*wave_points[x]);
    }
    ctxw.stroke();
    ctxw.strokeStyle = '#fff';
    ctxw.lineWidth=1.5;
    ctxw.moveTo(0, 0.5*height-scale_y*wave_points[0]);
    for (var x = 0; x < wave_points.length; x++) {
        ctxw.lineTo(x * offset, 0.5*height-scale_y*wave_points[x]);
    }
    ctxw.stroke();
        
    hbar_xoffset = 20;
    hbar_yoffset = 12;
    ctxw.beginPath();
    ctxw.strokeStyle = '#0000FF';
    ctxw.globalAlpha = 0.5;
    ctxw.lineWidth = 10;
    ctxw.moveTo(35,height-hbar_yoffset);
    ctxw.lineTo(35, (height-0.3*height)-hbar_yoffset);
    ctxw.moveTo(60,height-hbar_yoffset);
    ctxw.lineTo(60, (height-0.3*h3*height/thd_ref)-hbar_yoffset);
    ctxw.moveTo(80,height-hbar_yoffset);
    ctxw.lineTo(80, (height-0.3*h5*height/thd_ref)-hbar_yoffset);
    ctxw.moveTo(100,height-hbar_yoffset);
    ctxw.lineTo(100, (height-0.3*h7*height/thd_ref)-hbar_yoffset);
    ctxw.moveTo(120,height-hbar_yoffset);
    ctxw.lineTo(120, (height-0.3*h9*height/thd_ref)-hbar_yoffset);
    ctxw.moveTo(144,height-hbar_yoffset);
    ctxw.lineTo(144, (height-0.3*h11*height/thd_ref)-hbar_yoffset);
    ctxw.moveTo(163,height-hbar_yoffset);
    ctxw.lineTo(163, (height-0.3*h13*height/thd_ref)-hbar_yoffset);
    ctxw.stroke();
    ctxw.globalAlpha = 1;
    ctxw.fillStyle = '#3399FF';
    ctxw.shadowColor ='rgba(0,0,0,0)';
    ctxw.font = 'bold 10px Arial';
    ctxw.textAlign = 'left';
    if (thd_sel==1){ctxw.fillText('THD V',22, 199);}
    else{ctxw.fillText('THD I',22, 199);}
    ctxw.fillText('3°',57,199);
    ctxw.fillText('5°',77,199);
    ctxw.fillText('7°',97,199);
    ctxw.fillText('9°',117,199);
    ctxw.fillText('11°',137,199);
    ctxw.fillText('13°',157,199);
    ctxw.backgroundColor = '#1d1f20';
    
    var c = document.getElementById('myHysto');
    var ctxh = c.getContext('2d');
    c.setAttribute('width',d_width);
    var h_width = d_width.substring(0,3);
    var h_height = 200;
    var h_scale_y = 0.3;
    var h_x = 40;
    var h_offset = (1 / (h_values.length + 0.1*h_x+2)) * h_width;
    var h_gradient = ctxh.createLinearGradient(0, 0, 170, 0);
    h_gradient.addColorStop('0', '#0ff');
    h_gradient.addColorStop('0.5' ,'#33CCFF');
    h_gradient.addColorStop('1.0', '#3399FF');
        
    ctxh.clearRect(0, 0, h_width, h_height)
    ctxh.strokeRect(0, 0, h_width, h_height)
    ctxh.fill();
    myHysto.style.backgroundColor = '#1d1f20';
    ctxh.strokeStyle = '#3f4242';
    ctxh.lineWidth=1;
    for (var a=1; a<20; a++) {
        ctxh.beginPath();
        ctxh.moveTo(h_x,a*10);
        ctxh.lineTo(h_width, a*10);
        ctxh.stroke();
    }

    ctxh.strokeStyle = h_gradient;
    ctxh.lineWidth = 2;
    ctxh.shadowOffsetX = 0;
    ctxh.shadowOffsetY = 9;
    ctxh.shadowBlur = 20;
    ctxh.shadowColor = '#33CCFF';
    ctxh.beginPath();
    ctxh.moveTo(h_x, h_height-h_scale_y*h_values[0]);
    for (var x = 0; x < h_values.length; x++) {
        ctxh.lineTo(h_x+(x * h_offset), h_height-h_scale_y*h_values[x]);
    }
    ctxh.stroke();
        
    ctxh.fillStyle = '#3399FF';
    ctxh.shadowColor ='rgba(0,0,0,0)';
    ctxh.font = 'bold 10px Arial';
    ctxh.textAlign = 'left';
    ctxh.fillText(P_limit+'W',10, 25);
    ctxh.fillText(0.75*P_limit+'W',10,68);
    ctxh.fillText(0.5*P_limit+'W',10,112);
    ctxh.fillText(0.25*P_limit+'W',10,159);
    ctxh.font = 'bold 20px Arial';
    ctxh.textAlign = 'center';
    ctxh.fillText(energy+' Wh',h_width/2, 25);
    }
</script>
</body>
</hmtl>
)=====";
const char setting_webpage[] PROGMEM = 
R"=====(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset=utf-8>
      <title>ESP8266 Powermeter</title>
    </head>
    <body>
<style>
    html, body {
        font-family: Trebuchet MS, sans-serif;
        font-size: 16px;
        text-align: center;
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

    .box{
          display: flex;
          justify-content: center;
          max-width: 100%;
          padding: 130px 0px 0px 0px;
    }
    .box-form{
          max-width: 75%;
          min-width: 75%;
          float: left;
          padding: 0px 5px 5px 5px;
          max-height: 525px;
          min-height: 525px;
          margin: 10px 10px;
          background-color: #fff;
          box-shadow: 0 27px 87px rgba(0,0,0,0.5);
          border-radius: 15px;
          overflow: hidden;
    }
    .box-data{
          max-width: 30%;
          min-width: 30%;
          float: left;
          padding: 0px 5px 5px 5px;
          max-height: 525px;
          min-height: 525px;
          text-align: left;
          display: inline-block;
          margin: 5px 5px;
          background-color: #fff;
    }
    .box-form p{
        font-size: 20px;
        font-weight: bold;
        color: #696969;
        line-height: 5px;
        letter-spacing: 0px;
    }
    .bt_div{
        max-width: 100%;
        min-width: 100%;
        position: absolute;
        top: 85%;
        left: 0%;
        margin: 10px 10px;
        background-color:transparent;
        justify-content: center;
        text-align: center;
    }
    .button{
      border: 5em;
      cursor: pointer;
      outline: none;
      font-size: 12px;
      -webkit-transform: translate(0);
      transform: translate(0);
      background-image: linear-gradient(45deg, #4568dc, #b06ab3);
      padding: 0.7em 2em;
      border-radius: 65px;
      box-shadow: 2px 2px 15px rgba(0, 255, 255, 0.438);
      -webkit-transition: box-shadow 0.25s;
      transition: box-shadow 0.25s;
      color: white;
    }
    .button .text {
      background-clip: text;
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      background-image: linear-gradient(45deg, #4568dc, #b06ab3);
    }
    .button:after {
      content: "";
      border-radius: 18px;
      position: absolute;
      margin: 4px;
      top: 0;
      left: 0;
      bottom: 0;
      right: 0;
      z-index: -1;
      background: #0e0e10;
    }
    .button:hover {
      background-image: linear-gradient(-45deg, #fe0b60, #9f0bfe);
      box-shadow: 0 12px 24px rgba(128, 128, 128, 0.1);
    }
    .button:hover .text {
      background-image: linear-gradient(-45deg, #4568dc, #b06ab3);
    }
    .box-icons{
          width: calc(50% + 305px);
          justify-content: right;
          border: 0px transparent;
          display: inline-block;
    }
    .debug-box{
          max-width: calc(50% + 305px);
          text-align: left;
          display: inline-block;
          background-color:#696969;
    }
    .logo {
        position: absolute;
        top: 0px;
        left: calc(50% - 90px);
        padding: 5px 0px 0px 0px;
    }
    .title{
        position: absolute;
        width: 326px;
        top: 0px;
        left: calc(50% - 163px);
        padding: 70px 0px 0px 0px;
    }
    .setup_bt {
      width:90px;
      height:32px;
      float: right;
      color: #696969;
      cursor: pointer;
      text-align: right;
      border: 0px transparent;
      background:
      url('data:image/png;base64,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')
      no-repeat
      left center;
      padding: 0px 10px 0px 0px;
      background-size: 32px;
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
 <img class="logo" src='data:image/png;base64,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'>
 <h1 class='title'>System Setup</h1>
 <div class='lines'> 
  <div class='line'></div>
</div>
 <div class='box'>
  <div class='box-form'>
    <div class='box-data'>
        <p>General measuring:</p>
        <label for='measuring' id='measuring_rms'></label><br>
        <label for='measuring' id='measuring_pqs'></label><br>
        <br>
        <p>Basic settings:</p>
        <label for='ctr'>CT ratio (x:5A):</label><br>
        <input type='text' id='ctr' name='ctr' required
               minlength='1' maxlength='4' size='10'>
        <br>
        <label for='ptr'>PT ratio (x:1V):</label><br>
        <input type='text' id='ptr' name='ptr' required
               minlength='1' maxlength='5' size='10'>
        <br>
        <label for='name'>Meter Host Name:</label><br>
        <input type='text' id='name' name='name' required
           minlength='1' maxlength='18' size='10'>
        <br>
        <label for='ssid'>SSID:</label><br>
        <input type='text' id='ssid' name='ssid' required
           minlength='1' maxlength='18' size='10'>
        <br>
        <label for='password'>Password:</label><br>
        <input type='text' id='password' name='password' required
           minlength='1' maxlength='18' size='10'>
        <br>
        <br>
        Connect to DataBase: <input type='checkbox' id='db-checkbox' name='db-checkbox' unchecked>
    </div>
    <div class='box-data'>
        <p>Advaced measuring:</p>
        <label for='measuring' id='measuring_vpk'></label><br>
        <label for='measuring' id='measuring_ipk'></label><br>
        <br>
        <p>Advanced settings:</p>
        <label for='CH1OS'>CH1OS (Current Offset,±31):</label><br>
        <input type='text' id='ch1os' name='ch1os' required
           minlength='1' maxlength='3' size='10'>
        <br>
        <label for='CH2OS'>CH2OS (Voltage Offset,±31):</label><br>
        <input type='text' id='ch2os' name='ch2os' required
           minlength='1' maxlength='3' size='10'>
        <br>
        <label for='PHCAL'>PHCAL (Phase calibration,±31):</label><br>
        <input type='text' id='phcal' name='phcal' required
           minlength='1' maxlength='3' size='10'>
        <br>
        <label for='APOS'>APOS (Active Power offset,16-bit):</label><br>
        <input type='text' id='apos' name='apos' required
           minlength='1' maxlength='5' size='10'>
        <br>
        <label for='VRMSOS'>VRMSOS (V RMS OFFSET,12-bit):</label><br>
        <input type='text' id='vrmsos' name='vrmsos' required
           minlength='1' maxlength='5' size='10'>
        <br>
        <label for='IRMSOS'>IRMSOS (I RMS OFFSET,12-bit):</label><br>
        <input type='text' id='irmsos' name='irmsos' required
           minlength='1' maxlength='5' size='10'>       
        <br>
        <br>
    </div>
    <div class='box-data'>
        <p>Other measuring:</p>
        <label for='measuring' id='measuring_t'></label><br>
        <label for='measuring' id='measuring_f'></label><br>
        <br>
        <p>Parameters settings:</p>
        <label for='Current_function'>Current function (I=Ai+B):</label><br>
        <input type='text' id='iA' name='iA' required
           minlength='1' maxlength='8' size='10'>
        <input type='text' id='iB' name='iB' required
           minlength='1' maxlength='8' size='10'>
        <br>
        <label for='Voltage_function'>Voltage function (V=Av+B):</label><br>
        <input type='text' id='vA' name='vA' required
           minlength='1' maxlength='8' size='10'>
        <input type='text' id='vB' name='vB' required
           minlength='1' maxlength='8' size='10'>
        <br>
        <label for='Wave_I_offset'>Current Waveform offset:</label><br>
        <input type='text' id='wios' name='wios' required
           minlength='1' maxlength='8' size='10'>
        <br>
        <label for='Wave_V_offset'>Voltage Waveform offset:</label><br>
        <input type='text' id='wvos' name='wvos' required
           minlength='1' maxlength='8' size='10'>
        <br>
        <label for='Temperature_function'>Temperature function (T=At+B):</label><br>
        <input type='text' id='tA' name='tA' required
           minlength='1' maxlength='8' size='10'>
        <input type='text' id='tB' name='tB' required
           minlength='1' maxlength='8' size='10'>
        <br>
        <br>
        <br>
    </div>
  </div>
    <div class='bt_div'>
        <button class='button' id='bt_save'>Save</button>
        <button class='button' id='bt_refresh'>Refresh</button>
        <button class='button' id='bt_reboot'>Reboot</button>
    </div>
 </div>
 <div class='debug-box' id='debug'>
 </div>
 <div class='box-icons'>
  <form  class='form-icons' action='/'>
      <button class='home_bt' href='/' type='submit'>
          <img class='home_icon'/>Home<br>Page
      </button>
  </form>
</div>
<script>
    const saveElement = document.getElementById('bt_save');
    const refreshElement = document.getElementById('bt_refresh');
    const rebootElement = document.getElementById('bt_reboot');
    const dbchkElement = document.getElementById('db-checkbox');
    var dbchkValue;
    var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
    connection.onopen = function () {
        connection.send('Update');
    }
    connection.onmessage = function (event) {
        Manage_cmd();
    }
    connection.onerror = function (error) {
    }
    function Manage_cmd() {
        var data = event.data;
        var tmp = data.split('#');
        if (tmp[0]=='cmd'){
            document.getElementById('ctr').value = tmp[1];
            document.getElementById('ptr').value = tmp[2];
            document.getElementById('name').value = tmp[3];
            document.getElementById('ssid').value = tmp[4];
            document.getElementById('password').value = tmp[5];
            document.getElementById('ch1os').value = tmp[6];
            document.getElementById('ch2os').value = tmp[7];
            document.getElementById('phcal').value = tmp[8];
            document.getElementById('apos').value = tmp[9];
            document.getElementById('vrmsos').value = tmp[10];
            document.getElementById('irmsos').value = tmp[11];
            document.getElementById('iA').value = tmp[13];
            document.getElementById('iB').value = tmp[14];
            document.getElementById('vA').value = tmp[15];
            document.getElementById('vB').value = tmp[16];
            document.getElementById('wios').value = tmp[17];
            document.getElementById('wvos').value = tmp[18];
            document.getElementById('tA').value = tmp[19];
            document.getElementById('tB').value = tmp[20];
            if (tmp[12]==1){
                dbchkValue=1;
                document.getElementById('db-checkbox').checked = true;
            }
            if (tmp[12]==0){
                dbchkValue=0;
                document.getElementById('db-checkbox').checked = false;
            }
        }
        else{
            document.getElementById('measuring_rms').innerHTML = 'V: '+tmp[0]+'V'+'  I: '+tmp[1]+'A'+'  FP: '+tmp[9]+'%';
            document.getElementById('measuring_pqs').innerHTML = 'P: '+tmp[3]+'W'+'  Q: '+tmp[5]+'var'+'  S: '+tmp[4]+'va';
            var thd_sel = tmp[17];
            var wave_points = tmp[19].split('$');
            var max = wave_points.reduce(function(a, b) {
            return Math.max(a, b);
            }, -Infinity);
            var min = wave_points.reduce(function(a, b) {
            b=b*-1;
            return Math.max(a, b);
            }, -Infinity);
            if (thd_sel==1){
            document.getElementById('measuring_vpk').innerHTML = 'Voltage Peak: ' + min +'-'+ max;
            }
            else{
            document.getElementById('measuring_ipk').innerHTML = 'Current Peak: ' + min +'-'+ max;
            }
            document.getElementById('measuring_t').innerHTML ='Temp: '+ tmp[8] + '°C'
        }
    }
    saveElement.addEventListener('click', e =>{
        var ctrValue = document.getElementById('ctr').value;
        var ptrValue = document.getElementById('ptr').value;
        var nameValue = document.getElementById('name').value;
        var ssidValue = document.getElementById('ssid').value;
        var passwordValue = document.getElementById('password').value;
        var ch1osValue = document.getElementById('ch1os').value;
            if(ch1osValue>31){
                ch1osValue=31;
                document.getElementById('ch1os').value =ch1osValue;
            }
            if(ch1osValue<-31){
                ch1osValue=-31;
                document.getElementById('ch1os').value =ch1osValue;
            }
        var ch2osValue = document.getElementById('ch2os').value;
            if(ch2osValue>31){
                ch2osValue=31;
                document.getElementById('ch2os').value =ch2osValue;
            }
            if(ch2osValue<-31){
                ch2osValue=-31;
                document.getElementById('ch2os').value =ch2osValue;
            }
        var phcalValue = document.getElementById('phcal').value;
            if(phcalValue>31){
                phcalValue=31;
                document.getElementById('phcal').value =phcalValue;
            }
            if(phcalValue<-31){
                phcalValue=-31;
                document.getElementById('phcal').value =phcalValue;
            }
        var aposValue = document.getElementById('apos').value;
        var vrmsosValue = document.getElementById('vrmsos').value;
            if(vrmsosValue>2046){
                vrmsosValue=2046;
                document.getElementById('vrmsos').value =vrmsosValue;
            }
            if(vrmsosValue<-2046){
                vrmsosValue=-2046;
                document.getElementById('vrmsos').value =vrmsosValue;
            }
        var irmsosValue = document.getElementById('irmsos').value;
            if(irmsosValue>2046){
                irmsosValue=2046;
                document.getElementById('irmsos').value =irmsosValue;
            }
            if(irmsosValue<-2046){
                irmsosValue=-2046;
                document.getElementById('irmsos').value =irmsosValue;
            }
        var iAValue = document.getElementById('iA').value;
        var iBValue = document.getElementById('iB').value;
        var vAValue = document.getElementById('vA').value;
        var vBValue = document.getElementById('vB').value;
        var wiosValue = document.getElementById('wios').value;
        var wvosValue = document.getElementById('wvos').value;
        var tAValue = document.getElementById('tA').value;
        var tBValue = document.getElementById('tB').value;
        var mycmd='st#'+ ctrValue + '#' + ptrValue + '#' + nameValue + '#' + ssidValue + '#' + passwordValue + '#' + ch1osValue + '#' + ch2osValue + '#' + phcalValue + '#' + aposValue + '#' + vrmsosValue + '#' + irmsosValue + '#' + dbchkValue + '#' + iAValue+ '#' + iBValue + '#' + vAValue + '#' + vBValue + '#' + wiosValue + '#' + wvosValue + '#' + tAValue + '#' + tBValue;
        connection.send(mycmd);
    });
    refreshElement.addEventListener('click', e =>{
        connection.send('Update');
    });
    rebootElement.addEventListener('click', e =>{
        connection.send('Reboot');
    });
    dbchkElement.addEventListener('change', (e) =>{
      if (dbchkElement.checked) {
        dbchkValue=1;
      }
      else{
        dbchkValue=0;
      }
    });
</script>
</body>
</html>
)=====";

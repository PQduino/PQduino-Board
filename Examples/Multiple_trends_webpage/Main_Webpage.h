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
    .box-text{
          width: 50%;
          float: left;
          padding: 5px 35px 5px 35px;
          max-height: 500px;
          min-height: 500px;
          display: inline-block;
          margin: 10px 10px;
          background-color: #fff;
          box-shadow: 0 27px 87px rgba(0,0,0,0.5);
          border-radius: 15px;
          overflow: hidden;
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
      url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA7AAAAOwBeShxvQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAe2SURBVFiFpVZrcFXVFf7W2nufeyFQuFBMYBid1kZtS2kZSEtHFGwRxIEAFaVQqEXTa+lUwJaSZFowQjWEKlAZwQRK2qGpxUqIMD5G0zEjKMNr6Ggt4hM6mKoIgfC4yd2P1R83ZEhyE6yumTXnzD7r8e1v7b3OIvQgOU+/nkuCcQIaAgAEaRRCw/mp3/ioJ7//RyjbYv+n3hghJA8RUADgJRD+gwAC4UoANwmwj5hKT0//+j8/LwDuvDBo6xsLjKdnjFO1pz9pGawtv2Isf2w8f6Qdv3La02Bjqc6k6dlBf3vj3s8LoAMDQ2reXCAiyUCqkOHvR4ACYawI1QAAEWZD5GUwvAphhSd+moDHG+d8dd3nBnDVn98eIRKeEfANBF8BUBzAPiLeeezO/EPtNgiFEBQA0hKULmHvd7HQpKPzrvlM5WgHcPWmI8+BUIvAY4hlQKyvva3lXCyXQlgjwAQAgKA+cCjt3de/33rWbJNAp4RlDwkVvluUfysAfPmPR8a0Rxe+ngUfvFP0lRoQSTYADADDNr6Xa5wq6O/O/sl4KO3UAToRyzVp7DOW9+lUuE6nwnWRV3tjVje0nomu0FbtNx6qV1+72Th8Z9jG93IBIOZ1/KIah4T2tOTax9+b3R0DGgDQEsYZopfSlPiVFtwQQngEzGuMw1oA/xIyewGArCRBtFZ7Wh18KGfmOjoR/yUFNMBhLIAnDyevrr8kfv2wde+2AMjvDgADgLE8VDs6ZtKIRxbV/16QfyiyNEGL2aItVWovM2Mes7TjSi1mi0nTxIwNqk0acePoqLEyNFuCyGW0RwaMVxo+uEsvhXYMkwYsM4gUSyBSJBfX5KJNm1hAmawJXJebno0BfKgdD4k8txin5n23/OgI47iegsw1Hklt8VfjpSZynCSRHxtH9RkbNS/jw0Mji8bsDDCiHkAwACjPr0aOv7e7+MoK42hX3KspkVOl2vEiY9Xw3q00uncrjdYO39SWFyrHpTGnCo2jXbuLr6zQjm+KPL3aHQM9sdDO+fj7jx9kobJAMgPAgASdue0E+l2hgdUQmZixphfJSWl/3XysSfptA3AKQrVgWVpfNnTUpYG/X9Y4jYO/VojGAwCJ1AdWR/5RNqSuA8D2l7QsZaJHfCTXc5qqziNRG4fsp4Dy51YOvQMAJpV8MEKYZp9PJwoMQkuI5D62tAdQizrvLGZFRLgMhEcBAEJlRPLDbhkAgKlL/vuYAFdJb7mDUrQBQoogYwKwoa1e8wW0GyReesl8ukBPQnB0x+8H/6IruUKFSz48IETL2xhYtmNV3iigY0PqUJzc03mLjCMbNXNNXlNe0Y5VeXOU4+rIcSJynFCOq3esypuT15RXFDVzjXFk887k3dc1eSZlZGl5ZGlZmy7vnLwLAwCQTIo5E//k7yA0bv3DoJ9nCz1z4Yn1EAzp1/LF26uqyGYHkGFh5r0nDgDA1nWDuuy+CwMAUFVFNucc5kYWk390z8mvdf4+92enhkUWk4OTOT0nBwCSWODlscBZdw90M5AAwF3JU9sFEoRoX8eQ8m0C8eaqAdN7Tg7clTy5qO2vCoG0VlcNXPOpACR/cvJ2EC0VwrNZUQtuJcjyyuqBT3XxTYoR27SJBUoIY4VQ0+YzmwQvB4Inkyi6yF6HEpSVSaYxBYppT9BpbsqqnqAsxbIlV6nTTxhPA9nTW+ypcOPmASUbNw8oYU9TlaO3jaeBKnX6iWRSDHBJHwCEmt48UwXITyPbDEAA4URWbimAqGt363W2eROEYwn/hR80q1O5nsyahbPOZGYJK/VQXJpI9S1vUs3b4Jo3AbizHcDiGWcL4fnuxTPO7hQHYjCE0JS9BAwh6VK+yLOSgAPNOpXL3uxTgjWk9QIACMHPhUPDBZ8q0OD9TLgGaD8DQqXTzx8A4RAEI8q354wqmX6+goHJAHZ0ylMohJ3ltX2KL10snX6uBIL5omgaC34TRPYT0+sIUpkBTclAMpxBIwOjnLzUgbCBAWDZlAuFxhJiw3OSxhKWTblQqJ3aoC0l2KnKh7b3KXloe5+SKK2rtKUB3KLWd969sRQ3jqpXbss5pNM0Ie5pi2lFZSzNM2NOzdIOlXFPW7TFxJXbcg4ZR9XGUlwLhH5nU8sEOE57U79moeMELPvt8/FRK25pKQXJngduSdURQAhhKgkXL3++17HOANQlQ4d2QPCAVgQGcXAgJkLwAq0ys8RFe6q4+fyQIGpBm28xgAoAYPKPFr+Y07hiQupLxtMEALBKXlj6Qq/3s52LleNbSwQyH+BpkLAUTHvJy2vCqMxUGUki+pZARgH8IBDqCLShw0F6eFyrLG6IdducLicPj2v9CwhvechWJdQgwNpg3BYAYKvnErAoBLlRMc0WIH9xQ2xOh7tkLjM+XU4iR95YLkh9HHs/QlQQczwynooOx1PR4chygQly44WTsWPa8ajIkQc6NSJt6Z3HRtvV60f7YgDo7tmdxFKmyFhqHdTH16oL4W7jpXzhbtNv4W7TLwryoG4xswf18bXGUmssZYq6AIhZNUlb3aQtEhlA2Z/dyT0HySqnZhlHJ41Hvkrr7VUjpbhqpBSrtN5uPPKNo5PKqVn3HMy04s9c708j1cPlfhB6AQAEqXmv0QOdbf4HHIWD9jD51R0AAAAASUVORK5CYII=')
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
      url('data:image/png;base64,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')
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
  <div class='box-text' id='b-t'>
    <div class='lines'>
      <div class='line'></div>
    </div>
     <p>Voltage</p>
    <canvas id='V_trend' width='200' height='200' style='border:1px solid #d3d3d3;'> Your browser does not support the HTML5 canvas tag
        </canvas>
    <p>Current</p>
    <canvas id='I_trend' width='200' height='200' style='border:1px solid #d3d3d3;'> Your browser does not support the HTML5 canvas tag
        </canvas>
  </div>
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
    var a_width = properties.width;
    var v_values=[200];
    var i_values=[200];

    var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
    connection.onopen = function () {
        connection.send('Conectado  -  ' + new Date());
        console.log('Conectado  -  ' + new Date());
    }
        connection.onmessage = function (event) {
        console.log('Servidor (recibe): ', event.data);
        verValor();
    }
        connection.onerror = function (error) {
        console.log('WebSocket Error!!!', error);
    }
    function verValor() {
    var data = event.data;
    var tmp = data.split('#');

    var V_val = tmp[0];
    var V_limit = 300;
    v_values.shift();
    v_values[199] = V_val*2;
    
    var I_val = tmp[1];
    var I_limit = 1.5;
    i_values.shift();
    i_values[199] = I_val*400/1.5;
    
    var a = document.getElementById('V_trend');
    var atxh = a.getContext('2d');
    a.setAttribute('width',a_width);
    var v_width = a_width.substring(0,3);
    var v_height = 200;
    var v_scale_y = 0.3;
    var v_x = 40;
    var v_offset = (1 / (v_values.length + 0.1*v_x+2)) * v_width;
    var v_gradient = atxh.createLinearGradient(0, 0, 170, 0);
    v_gradient.addColorStop('0', '#0ff');
    v_gradient.addColorStop('0.5' ,'#33CCFF');
    v_gradient.addColorStop('1.0', '#3399FF');
        
    atxh.clearRect(0, 0, v_width, v_height)
    atxh.strokeRect(0, 0, v_width, v_height)
    atxh.fill();
    V_trend.style.backgroundColor = '#1d1f20';
    atxh.strokeStyle = '#3f4242';
    atxh.lineWidth=1;
    for (var a=1; a<20; a++) {
        atxh.beginPath();
        atxh.moveTo(v_x,a*10);
        atxh.lineTo(v_width, a*10);
        atxh.stroke();
    }

    atxh.strokeStyle = v_gradient;
    atxh.lineWidth = 2;
    atxh.shadowOffsetX = 0;
    atxh.shadowOffsetY = 9;
    atxh.shadowBlur = 20;
    atxh.shadowColor = '#33CCFF';
    atxh.beginPath();
    atxh.moveTo(v_x, v_height-v_scale_y*v_values[0]);
    for (var x = 0; x < v_values.length; x++) {
        atxh.lineTo(v_x+(x * v_offset), v_height-v_scale_y*v_values[x]);
    }
    atxh.stroke();
        
    atxh.fillStyle = '#3399FF';
    atxh.shadowColor ='rgba(0,0,0,0)';
    atxh.font = 'bold 10px Arial';
    atxh.textAlign = 'left';
    atxh.fillText(V_limit+'V',10, 12);
    atxh.fillText(0.75*V_limit+'V',10,12+0.25*v_height);
    atxh.fillText(0.5*V_limit+'V',10,12+0.5*v_height);
    atxh.fillText(0.25*V_limit+'V',10,12+0.75*v_height);
    atxh.font = 'bold 20px Arial';
    atxh.textAlign = 'center';
    atxh.fillText(V_val+' V',v_width/2, 25);
    
    var b = document.getElementById('I_trend');
    var btxh = b.getContext('2d');
    b.setAttribute('width',a_width);
    var i_width = a_width.substring(0,3);
    var i_height = 200;
    var i_scale_y = 0.3;
    var i_x = 40;
    var i_offset = (1 / (i_values.length + 0.1*i_x+2)) * i_width;
    var i_gradient = btxh.createLinearGradient(0, 0, 170, 0);
    i_gradient.addColorStop('0', '#0ff');
    i_gradient.addColorStop('0.5' ,'#33CCFF');
    i_gradient.addColorStop('1.0', '#3399FF');
        
    btxh.clearRect(0, 0, i_width, i_height)
    btxh.strokeRect(0, 0, i_width, i_height)
    btxh.fill();
    I_trend.style.backgroundColor = '#1d1f20';
    btxh.strokeStyle = '#3f4242';
    btxh.lineWidth=1;
    for (var a=1; a<20; a++) {
        btxh.beginPath();
        btxh.moveTo(i_x,a*10);
        btxh.lineTo(i_width, a*10);
        btxh.stroke();
    }

    btxh.strokeStyle = i_gradient;
    btxh.lineWidth = 2;
    btxh.shadowOffsetX = 0;
    btxh.shadowOffsetY = 9;
    btxh.shadowBlur = 20;
    btxh.shadowColor = '#33CCFF';
    btxh.beginPath();
    btxh.moveTo(i_x, i_height-i_scale_y*i_values[0]);
    for (var x = 0; x < v_values.length; x++) {
        btxh.lineTo(i_x+(x * i_offset), i_height-i_scale_y*i_values[x]);
    }
    btxh.stroke();
        
    btxh.fillStyle = '#3399FF';
    btxh.shadowColor ='rgba(0,0,0,0)';
    btxh.font = 'bold 10px Arial';
    btxh.textAlign = 'left';
    btxh.fillText(I_limit+'A',10, 12);
    btxh.fillText(0.75*I_limit+'A',10,12+0.25*i_height);
    btxh.fillText(0.5*I_limit+'A',10,12+0.5*i_height);
    btxh.fillText(0.25*I_limit+'A',10,12+0.75*i_height);
    btxh.font = 'bold 20px Arial';
    btxh.textAlign = 'center';
    btxh.fillText(I_val+' A',i_width/2, 25);
    }
</script>
</body>
</hmtl>
)=====";

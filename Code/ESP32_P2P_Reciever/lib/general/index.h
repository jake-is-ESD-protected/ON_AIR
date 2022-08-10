#ifndef _INDEX_H_
#define _INDEX_H_

const char index[] = R"=====(<!DOCTYPE html>
<html>
<style>
html{ 
    font-family: Helvetica;
    display: inline-block; 
    margin: 0px auto; 
    text-align: center;
}
body{margin-top: 50px;} 
h1{color: #444444;margin: 50px auto 30px;} 
h3{color: #444444;margin-bottom: 50px;}
p{font-size: 14px;color: #888;margin-bottom: 10px;}
.button {
    display: block;
    width: 80px;
    background-color: #3498db;
    border: none;
    color: white;
    padding: 13px 30px;
    text-decoration: none;
    font-size: 25px;
    margin: 0px auto 35px;
    cursor: pointer;
    border-radius: 4px;
}
.button-on {background-color: #3498db;}
.button-on:active {background-color: #2980b9;}
.button-off {background-color: #34495e;}
.button-off:active {background-color: #2c3e50;}
</style>

<body>
    <h1>[ONAIR]-Webserver</h1>
    <h1>Sensor Value:<span id="ADCValue">0</span></h1><br>
    <a class="button button-on" href="/idle">idle</a>
    <a class="button button-on" href="/woex">woex</a>
    <a class="button button-on" href="/meet">meet</a>
    <a class="button button-on" href="/reco">reco</a>
    <a class="button button-on" href="/wait">wait</a>
    <a class="button button-on" href="/welc">wait</a>

    <script>
    setInterval(function() {
    // Call a function repetatively with 2 Second interval
    getData();
}, 2000); //2000mSeconds update rate

function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
        document.getElementById("ADCValue").innerHTML =
        this.responseText;
        }
    };
    xhttp.open("GET", "readADC", true);
    xhttp.send();
}
</script>


</body>
</html>)=====";

#endif
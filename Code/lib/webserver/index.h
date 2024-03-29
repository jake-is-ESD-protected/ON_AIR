#ifndef _INDEX_H_
#define _INDEX_H_

const char ws_index[] = R"=====(
<!DOCTYPE html>
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
    width: 160px;
    height: 90px;
    background-color: #34495e;
    border: none;
    color: white;
    padding: 8px 10px 10px 10px;
    text-decoration: none;
    font-size: 25px;
    margin: 0px auto 35px;
    cursor: pointer;
    border-radius: 4px;
    text-align: center;
}
</style>

<body>
    <h1>[ONAIR]-Webserver (OTA edition)</h1>

    <button class="button" id="idle" onclick="setState('idle')" style="background-color: #3498db;">Idle</button>
    <button class="button" id="woex" onclick="setState('woex')">Working</button>
    <button class="button" id="meet" onclick="setState('meet')">Meeting</button>
    <button class="button" id="reco" onclick="setState('reco')">Recording</button>
    <button class="button" id="wait" onclick="setState('wait')">Please wait</button>
    <button class="button" id="welc" onclick="setState('welc')">Welcome</button>
    <button class="button" id="bell">Bell</button>

    <script>
    setInterval(function() {
    // Call a function repetatively with 2 Second interval
    getState();
}, 2000); //2000mSeconds update rate


function setAllButtonsLow() {
    var buttons = document.getElementsByClassName("button");
    for(var i = 0, l = buttons.length; i < l; i++) {
        var button = buttons[i];
        button.style.backgroundColor="#34495e";
      }
}


function setButtonHigh(id) {
    document.getElementById(id).style.backgroundColor="#3498db";
}


function setState(state) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", state, true);
    xhttp.send();
}


function getState() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText == "bell") {
                alert("Somebody is calling!");
            }
            else {
                setAllButtonsLow();
                setButtonHigh(this.responseText);
            }
        }
    };
    xhttp.open("GET", "getState", true);
    xhttp.send();
}

function getBell() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById(this.responseText).style.backgroundColor="red";
        }
    };
    xhttp.open("GET", "getBell", true);
    xhttp.send();
}
</script>


</body>
</html>
)=====";

#endif
setInterval(function() {
    // Call a function repetatively with 2 Second interval
    getData();
}, 2000); //2000mSeconds update rate


function setAllButtonsLow() {
    var nas = document.getElementsByTagName("a");
    for(var i = 0, l = nas.length; i < l; i++) {
        var na = nas[i];
        na.style.backgroundColor="grey";
      }
}

function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            setAllButtonsLow();
            document.getElementById(this.responseText).style.backgroundColor="pink";
        }
    };
    xhttp.open("GET", "getState", true);
    xhttp.send();
}
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
.card{
    max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}

.subcard{
     min-height: 50px;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}

.con-valid {
  background: green;
}

.con-fail{
  background: red;
}

</style>
<body>

<div class="card">
  <h4>WiFi communication with the ESP32 </h4>
  (every 500ms) <br>
  <h1>Value: <span id="ReadValue">0</span></h1><br>

  <h5> Cimbali Group</h5><br>
<div class="subcard con-valid">
Connection Working!
</div>
<div class="subcard con-fail">
Connection failed.
</div>
</div>
<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 500); //2000mSeconds update rate
var l = 900;
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ReadValue").innerHTML =
      this.responseText;
      document.getElementsByClassName("con-fail")[0].hidden=true;
      document.getElementsByClassName("con-valid")[0].hidden=false;
      // adding some colour in the background that change
      document.getElementsByClassName("card")[0].style.backgroundColor="#"+(++l).toString(16);
    }
  };
  xhttp.timeout = 1000; // time in milliseconds 
  xhttp.ontimeout = function() { 
        console.log("Not Connected"); 
        document.getElementsByClassName("card")[0].style.backgroundColor="red";
        document.getElementsByClassName("con-fail")[0].hidden=false;
        document.getElementsByClassName("con-valid")[0].hidden=true;
        }
  xhttp.open("GET", "readData", true);
  xhttp.send();
}
</script>
</body>
</html>
)=====";

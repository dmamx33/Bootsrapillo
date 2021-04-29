function turnOnRelay(relay){
  
  if (relay < 1 || relay > 5){ 
    console.log("Incorrect Paramter");
    return;
  }

  console.log("Maybe we'll turn on a relay.");
  request =  new XMLHttpRequest();   // Starts request
  home_page = "/relay.html"
  request_data = home_page + "/RELAY_" + relay + "_ON"; // Formats the request - remember it's all text
  console.log(request_data);
  // request.open(method, url, async);
  request.open("GET", request_data, true); // "GET" command ran in background
  request.send(null);                     // Sends request
  console.log("AJAX Request Sent.");
  buttonStateChange(relay);

}

function buttonStateChange(relay){

  if (relay < 1 || relay > 5){ 
    console.log("Incorrect Paramter");
    return; 
  } 
 
  id_text = "#relay" + relay;

  if (relay <= 4){
    if ($(id_text).text() == "On"){
      $(id_text).html("Off");
    }
    else if ($(id_text).text() == "Off"){
      $(id_text).html("On");
    }
  }

  else {
    if ($("#totalControl").text() == "All On"){
      $("#totalControl").html("All Off");
    }
    else if ($("#totalControl").text() == "All Off"){
      $("#totalControl").html("All On");
    }
  }
}


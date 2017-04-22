'use strict';

setInterval(UpdateData, 300);

var MAX_X_COORD = 200;
var MAX_Y_COORD = 200;
var MAX_Z_COORD = 200;

var xCoord = 0;
var yCoord = 0;
var zCoord = 0;

var eSetTemp = 0;
var tSetTemp = 0;

var rxLogFlag = false;
var txLogFlag = false;

function EnableRxLog(){
    var checkbox = document.getElementById("rx_checkbox");
    if (checkbox.checked){
        rxLogFlag = true;
    }
    else {
        rxLogFlag = false;
    }
}

function EnableTxLog(){
    var checkbox = document.getElementById("tx_checkbox");
    if (checkbox.checked){
        txLogFlag = true;
    }
    else {
        txLogFlag = false;
    }
}

function UpdateData(){
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'control?cmd=log&value=read', false);
    xhr.send();
    if (xhr.status === 200){
        var logWindow = document.getElementById("uart_log").contentWindow;
        var logDocument = logWindow.document;
        var updateFlag = false;
        var subStrings = xhr.responseText.split('\n');
        for (var i = 0; i < subStrings.length; i++){
            if (subStrings[i].length > 0) {
                if ((txLogFlag) && (subStrings[i].charAt(0) === '<')) {
                    logDocument.body.appendChild(document.createTextNode(subStrings[i]));
                    logDocument.body.appendChild(document.createElement("br"));
                    updateFlag = true;
                }
                if ((rxLogFlag) && (subStrings[i].charAt(0) !== '<')) {
                    logDocument.body.appendChild(document.createTextNode(subStrings[i]));
                    logDocument.body.appendChild(document.createElement("br"));
                    updateFlag = true;
                }
            }
        }
        if (updateFlag) logWindow.scrollBy(0, logWindow.innerHeight);
    }
    
    xhr = new XMLHttpRequest();
    xhr.open('GET', 'control?cmd=status&value=read', false);
    xhr.send();
    if (xhr.status === 200){
        var status_label = document.getElementById("status_label");
        var status = JSON.parse(xhr.responseText);
        if (status.uart_bridge === "true") {
            status_label.textContent = "TCP-UART bridge";
        }
        else if (status.print_server === "true") {
            if (status.file_name !== ""){
                status_label.textContent = "print file " + status.file_name;
            }
            else{
                status_label.textContent = "busy";
            }
        }
        else {
            status_label.textContent = "ready";
        }
    }
}

function SendGCode(){
    var gcodeString = document.getElementById("gcode_str").value;
    var xhr = new XMLHttpRequest();
    
    xhr.open('GET', 'control?cmd=gcode&value=' + gcodeString, false);
    xhr.send();
};

function SendControlCmd(id){
    //var e_sel = document.getElementById("extruder_select").options.selectedIndex;
    var xhr = new XMLHttpRequest();
    var cmd = "";
    switch(id){
        case "home_x":
            xCoord = 0;
            cmd = "G28 X";
            break;
            
        case "home_y":
            yCoord = 0;
            cmd = "G28 Y";
            break;
            
        case "home_z":
            zCoord = 0;
            cmd = "G28 Z";
            break;
            
        case "home_all":
            xCoord = 0;
            yCoord = 0;
            zCoord = 0;
            cmd = "G28";
            break;
        
        case "fast_left_x":
            xCoord -= 20;
            if (xCoord < 0) xCoord = 0;
            cmd = "G1 F1000 X" + xCoord;
            break;
            
        case "left_x":
            xCoord -= 1;
            if (xCoord < 0) xCoord = 0;
            cmd = "G1 F1000 X" + xCoord;
            break;
        
        case "right_x":
            xCoord += 1;
            if (xCoord > MAX_X_COORD) xCoord = MAX_X_COORD;
            cmd = "G1 F1000 X" + xCoord;
            break;
            
        case "fast_right_x":
            xCoord += 20;
            if (xCoord > MAX_X_COORD) xCoord = MAX_X_COORD;
            cmd = "G1 F1000 X" + xCoord;
            break;
        
        case "fast_down_y":
            yCoord -= 20;
            if (yCoord < 0) yCoord = 0;
            cmd = "G1 F1000 Y" + yCoord;
            break;
            
        case "down_y":
            yCoord -= 1;
            if (yCoord < 0) yCoord = 0;
            cmd = "G1 F1000 Y" + yCoord;
            break;
        
        case "up_y":
            yCoord += 1;
            if (yCoord > MAX_Y_COORD) yCoord = MAX_Y_COORD;
            cmd = "G1 F1000 Y" + yCoord;
            break;
            
        case "fast_up_y":
            yCoord += 20;
            if (yCoord > MAX_Y_COORD) yCoord = MAX_Y_COORD;
            cmd = "G1 F1000 Y" + yCoord;
            break;
        
        case "fast_down_z":
            zCoord -= 20;
            if (zCoord < 0) zCoord = 0;
            cmd = "G1 F1000 Z" + zCoord;
            break;
            
        case "down_z":
            zCoord -= 1;
            if (zCoord < 0) zCoord = 0;
            cmd = "G1 F1000 Z" + zCoord;
            break;
        
        case "up_z":
            zCoord += 1;
            if (zCoord > MAX_Z_COORD) zCoord = MAX_Z_COORD;
            cmd = "G1 F1000 Z" + zCoord;
            break;
            
        case "fast_up_z":
            zCoord += 20;
            if (zCoord > MAX_Z_COORD) zCoord = MAX_Z_COORD;
            cmd = "G1 F1000 Z" + zCoord;
            break;
            
            
        case "fast_back_e":
            cmd = "G1 E100";
            break;
            
        case "back_e":
            cmd = "G1 F1000 Z" + zCoord;
            break;
        
        case "forward_e":
            cmd = "G1 E50";
            break;
            
        case "fast_forward_e":
            cmd = "G1 E5";
            break;
            
        case "extruder_low":
            eSetTemp = parseInt(document.getElementById("set_e_temp").value);
            eSetTemp -= 1;
            document.getElementById("set_e_temp").value = eSetTemp;
            cmd = "M104 S" + eSetTemp;
            break;
            
        case "extruder_high":
            eSetTemp = parseInt(document.getElementById("set_e_temp").value);
            eSetTemp += 1;
            document.getElementById("set_e_temp").value = eSetTemp;
            cmd = "M104 S" + eSetTemp;
            break;
            
        case "table_low":
            tSetTemp = parseInt(document.getElementById("set_t_temp").value);
            tSetTemp -= 1;
            document.getElementById("set_t_temp").value = tSetTemp;
            cmd = "M140 S" + tSetTemp;
            break;
            
        case "table_high":
            tSetTemp = parseInt(document.getElementById("set_t_temp").value);
            tSetTemp += 1;
            document.getElementById("set_t_temp").value = tSetTemp;
            cmd = "M140 S" + tSetTemp;
            break;
    }
    
    xhr.open('GET', 'control?cmd=gcode&value=' + cmd, false);
    xhr.send();
}

function SetTcpUartBridgeState(){
    var button = document.getElementById("tcp_uart_bridge");
    var xhr = new XMLHttpRequest();
    
    if (button.innerHTML.trim() === "TCP-UART bridge: Disable"){
        xhr.open('GET', 'control?cmd=uart&value=enable', false);
        xhr.send();
        if (xhr.status === 200){
            button.innerHTML = "TCP-UART bridge: Enable";
        }
    }
    else {
        xhr.open('GET', 'control?cmd=uart&value=disable', false);
        xhr.send();
        if (xhr.status === 200){
            button.innerHTML = "TCP-UART bridge: Disable";
        }
    }
}

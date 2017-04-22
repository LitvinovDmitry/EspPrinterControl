/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

window.onload = Init;

var apFlag = false;

function Init(){
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'connection', false);
    xhr.send();
    if (xhr.status === 200){
        var connection = JSON.parse(xhr.responseText);
        document.getElementById("ssid").value = connection.ssid;
        document.getElementById("pass").value = connection.pass;
        document.getElementById("name").value = connection.name;
        var ip_str = (0xFF & (connection.ip >> 24)).toString();
        ip_str += "." + (0xFF & (connection.ip >> 16)).toString();
        ip_str += "." + (0xFF & (connection.ip >> 8)).toString();
        ip_str += "." + (0xFF & connection.ip).toString();
        document.getElementById("ip").value = ip_str;
        document.getElementById("web_server_port").value = connection.web_server_port;
        document.getElementById("tcp_uart_port").value = connection.tcp_uart_port;
        if (connection.wifi_mode === 'ap') apFlag = true;
        else apFlag = false;
        document.getElementById("ap_checkbox").checked = apFlag;
    }
}

function EnableAP(){
    var checkbox = document.getElementById("ap_checkbox");
    if (checkbox.checked){
        apFlag = true;
    }
    else {
        apFlag = false;
    }
}

function SaveConnection(){
    var ssid = document.getElementById("ssid").value;
    var pass = document.getElementById("pass").value;
    var name = document.getElementById("name").value;
    var ip_str = document.getElementById("ip").value;
    var web_server_port = document.getElementById("web_server_port").value;
    var tcp_uart_port = document.getElementById("tcp_uart_port").value;
    var byte_str = ip_str.split('.');
    var ip = 0;
    for (var i = 0; i < byte_str.length; i++){
        ip |= parseInt(byte_str[i]) << (24 - i * 8);
    }
    var wifi_mode = 'st';
    if (apFlag) wifi_mode = 'ap';
    var xhr = new XMLHttpRequest();
    xhr.open('POST', 'connection?ssid=' + ssid + '&pass=' + pass + '&name=' + name + '&ip=' + ip + '&web_server_port=' + web_server_port + '&tcp_uart_port=' + tcp_uart_port + '&wifi_mode=' + wifi_mode, false);
    xhr.send();
}


function ResetEsp(){
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'control?cmd=reset&value=1', false);
    xhr.send();
}
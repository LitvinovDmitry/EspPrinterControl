'use strict';

window.onload = Init;
var currentPath = "";

function Init(){
    UpdateFileList();
//    var FilesTable = document.getElementById("file_list");
//    FilesTable.onclick = HighlightFile;
}

function HighlightFile(e){
    var FilesTable = document.getElementById('file_list');
    var selected = FilesTable.getElementsByClassName('selected');
    if (selected[0]) selected[0].className = '';
    if ('TR' === e.target.parentNode.nodeName) e.target.parentNode.className = 'selected';
}

function ChangeDir(e){
    if ('TR' === e.target.parentNode.nodeName){
        var tableRow = e.target.parentNode;
        if (tableRow.cells[1].innerText === "<DIR>"){
            var dirName = tableRow.cells[0].innerText;
            if (dirName !== ".."){
                if (currentPath !== "") currentPath += '/';
                currentPath += dirName;
            }
            else {
                if (currentPath.lastIndexOf("/") !== -1){
                    currentPath = currentPath.substring(0, currentPath.lastIndexOf("/"));
                }
                else {
                    currentPath = "";
                }
            }
            UpdateFileList();
        };
    };
}

function UpdateFileList(){
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'gcode?path=' + currentPath, false);
    xhr.send();
    if (xhr.status === 200){
        var oldFilesTable = document.getElementById("file_list");
        var newFilesTable = document.createElement("tbody");
        newFilesTable.setAttribute("id", "file_list");
        newFilesTable.onclick = HighlightFile;
        newFilesTable.ondblclick = ChangeDir;
        var files = JSON.parse(xhr.responseText);
        if (currentPath !== ""){
            var row = document.createElement("tr");
            var nameCol = document.createElement("td");
            nameCol.appendChild(document.createTextNode(".."));
            row.appendChild(nameCol);
            var sizeCol = document.createElement("td");
            sizeCol.appendChild(document.createTextNode("<DIR>"));
            row.appendChild(sizeCol);
            var dateCol = document.createElement("td");
            dateCol.appendChild(document.createTextNode(""));
            row.appendChild(dateCol);
            newFilesTable.appendChild(row);
        }
        for (var i = 0; i < files.length; i++){
            var row = document.createElement("tr");
            var nameCol = document.createElement("td");
            nameCol.appendChild(document.createTextNode(files[i].name));
            row.appendChild(nameCol);
            var sizeCol = document.createElement("td");
            sizeCol.appendChild(document.createTextNode(files[i].size));
            row.appendChild(sizeCol);
            var dateCol = document.createElement("td");
            dateCol.appendChild(document.createTextNode(files[i].date));
            row.appendChild(dateCol);
            newFilesTable.appendChild(row);
        }
        oldFilesTable.parentNode.replaceChild(newFilesTable, oldFilesTable);
    }
}

function LoadFile(){
    var input = document.getElementById('file_name');
    var button = document.getElementById('load_button');
    var file = input.files[0];
    var fileName = currentPath;
    if (fileName !== "") fileName += "/";
    fileName += file.name;
    if (file) {
        var xhr = new XMLHttpRequest();
        
        xhr.upload.onprogress = function(event) {
            var progress = 100 * event.loaded / event.total;
            button.value = "Load: " + Math.round(progress) + "%";
        };
        xhr.upload.onload = function() {
            button.value = "Load: 100%";
        };
        
        xhr.onload = xhr.onerror = function() {
            button.value = "Load file";
            if (this.status === 200) {
                UpdateFileList();
            } 
            else {
            };
        };
        var formData = new FormData();
        formData.append('file', file, fileName);
        xhr.open('POST', 'gcode', true);
        xhr.send(formData);
    }
}

function DeleteFile(){
    var FilesTable = document.getElementById('file_list');
    var selected = FilesTable.getElementsByClassName('selected');
    if (selected[0]){
        var nameCell = selected[0].cells[0];
        var fileName = nameCell.innerText;
        if (currentPath !== "") fileName = currentPath + "/" + fileName;
        var xhr = new XMLHttpRequest();
        xhr.open('DELETE', 'gcode?name='+fileName, false);
        xhr.send();
        if (xhr.status === 200){
            UpdateFileList();
        }
        else{
        }
    }
}

function CreateDir(){
    var dirName = prompt("Enter directory name", "");
    if ((dirName !== null) && (dirName.length > 0)){
        if (currentPath !== "") dirName = currentPath + "/" + dirName;
        var xhr = new XMLHttpRequest();
        xhr.open('PUT', 'gcode?name='+dirName, false);
        xhr.send();
        if (xhr.status === 200){
            UpdateFileList();
        }
        else{
        }
    }
}

function PrintFile(){
    var FilesTable = document.getElementById('file_list');
    var selected = FilesTable.getElementsByClassName('selected');
    if (selected[0]){
        var nameCell = selected[0].cells[0];
        var fileName = nameCell.innerText;
        if (currentPath !== "") fileName = currentPath + "/" + fileName;
        var xhr = new XMLHttpRequest();
        xhr.open('GET', 'control?cmd=print&value='+ fileName, false);
        xhr.send();
    }
}
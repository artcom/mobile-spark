<?php
    $method = $_SERVER['REQUEST_METHOD']; 
    switch($method) { 
    case "PUT": 
        print($method."->update"."\n"); 
	    $id=$_GET["id"];
	    $value=$_GET["value"];
        if (!empty($value)) {
            print("id = ".$id." value = ".$value."\n");
        }
        //this does not work with mobile-spark but with curl from command-line
        $putdata = file_get_contents("php://input");
        print("putdata ".$putdata."\n");
        //parse_str(file_get_contents("php://input"),$put_vars);
        //print("put id: ".$put_vars["id"]."\n");
        //print("put value: ".$put_vars["value"]."\n");
        break; 
    case "GET": 
        print($method."->read"."\n"); 
	    $id=$_GET["id"];
	    $value=$_GET["value"];
        if (!empty($value)) {
            print("id = ".$id." value = ".$value."\n");
        }
        break; 
    case "DELETE": 
        print($method."->delete"."\n"); 
	    $id=$_GET["id"];
	    $value=$_GET["value"];
        if (!empty($value)) {
            print("id = ".$id." value = ".$value."\n");
        }
        break; 
	case "POST": 
        print($method."->create"."\n"); 
	    $id=$_GET["id"];
	    $value=$_GET["value"];
        if (!empty($value)) {
            print("id = ".$id." value = ".$value."\n");
        }
        print("post id: ".$_POST["id"]."\n");
        print("post value: ".$_POST["value"]."\n");
        break; 
    }
?>

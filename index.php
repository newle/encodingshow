<html>
<head>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
    <title>字符编码</title>
    <meta http-equiv="Content-Type" content="text/html; charset=gb18030">
    <style>
        input {
            border-left: 0px;
            border-right: 0px;
            border-top: 0px;
            border-bottom: 1px solid;
			width: 300px;
        }
		#inst {
			width: 80px;
		}
		.left {
			float:left;
			width: 50%;
			text-align: center;
		}
		.right {
			float:right;
			width: 50%;
		}
		hr {
			clear: right;
		} 
		#maintable {
			position: fixed;
			left: 28%;
			top: 70px;
			width: 800px;
			margin: 0 auto;
		}

		h1 {
			text-align: center;
		}
		#submitresult {
			background-color: #A1EDE6;
			width:33%;
			margin: 0 1px 0 1px;
			float:left;
			text-align: left;
			border: 1px solid;
		}
		#submitresulthead {
			background-color: #B4E5BB;
			width:100%;
			margin: 0 1px 0 1px;
			float:left;
		/*	text-align: center;*/
			border: 1px solid;
		}
		.clear {
			clear: both;
		}
    </style>
</head>

<body>
    <h1>字符编码</h1>
	<div id="maintable">
    <form action="index.php" method="get">
		<div class="left">
			<table>
            <tr><td> <input type="text" name="pic_url" value="必填" onclick="if(this.value=='必填'){this.value='';}"> </td></tr>
			</table>
		</div>
		<div class="left">
            <input id="inst" type="submit" name="read" value="查看编码"/><br />
            <!--<input id="local" type="submit" name="local_pup" value="发给local pup"/><br />
            <input id="norm" type="submit" name="norm_pup" value="发给norm pup"/><br />-->
		</div>

    </form>
	</div>
	<div class="clear" style="height:70px;"></div>
	<div id="show">
		<?php
		echo '<div id="submitresulthead" style="word-break:break-all;">';
		if(!empty($_GET["pic_url"])) { 

			$t = time();
			if(isset($_GET["read"])) {
				system("sh ./demo_encode.sh \"".$_GET["pic_url"]."\" ".$t, $retval);
				//sleep(3);
			}


			$result_url = $t."norm.log";
			$fp = fopen($result_url, "r");
			echo "<hr/>";
			while ($fp and !feof($fp)) {
				$line = fgets($fp);
				if(strrpos($line, "LocalIndex") !== false) {
					echo '</div><div id="submitresult">';
				}
				if(strrpos($line, "spawn") === false) {
					echo $line."<br/>";
				}
				
			}

			echo '</div>';

			//system("rm -f ".$t."*.log", $retval);
		} else {
			echo "请输入你想查询的文本内容<br/>";
		}
		echo '</div>';
		?>
	</div>
	<script type="script/javascript">
		document.onkeydown = function (e) { 
			var theEvent = window.event || e; 
			var code = theEvent.keyCode || theEvent.which; 
			if (code == 13) { 
				$("#inst").submit();
			} 
		};
	</script>

</body>
</html>


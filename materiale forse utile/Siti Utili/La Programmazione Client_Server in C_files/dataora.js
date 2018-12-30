			var dayarray	= new Array("Domenica","Luned&igrave","Marted&igrave","Mercoled&igrave","Gioved&igrave","Venerd&igrave","Sabato")
			var montharray	= new Array("Gennaio","Febbraio","Marzo","Aprile","Maggio","Giugno","Luglio","Agosto","Settembre","Ottobre","Novembre","Dicembre")
			
			function getthedate()
			 {
				var mydate	= new Date()
				var year	= mydate.getYear()
				if (year < 1000) year+=1900
				var day		= mydate.getDay()
				var month	= mydate.getMonth()
				var daym	= mydate.getDate()
				var hours	= mydate.getHours()
				var minutes	= mydate.getMinutes()
				var seconds	= mydate.getSeconds()
				if (hours <= 9) hours="0"+hours
				if (minutes <= 9) minutes="0"+minutes
				if (seconds <= 9) seconds="0"+seconds
				var cdate="<small><font color='000000' face='Arial'><b>"+dayarray[day]+
				" "+daym+" "+montharray[month]+" "+year+" "+hours+":"+minutes+":"+
				seconds+" "+"</b></font></small>"
				if (document.all) document.all.clock.innerHTML = cdate
				else if (document.getElementById)
							document.getElementById("clock").innerHTML = cdate
				else document.write(cdate)
			 }
			if (!document.all&&!document.getElementById) getthedate()
			function goforit()
			 {
				if (document.all||document.getElementById)
				setInterval("getthedate()",1000)
			 }
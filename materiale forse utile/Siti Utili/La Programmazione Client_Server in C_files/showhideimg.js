			function ShowImg(id)
			 {
			 	var x,y;
				if (self.innerHeight) // all except Explorer
				{
					x = self.innerWidth;
					y = self.innerHeight;
				}
				else if (document.documentElement && document.documentElement.clientHeight)
					// Explorer 6 Strict Mode
				{
					x = document.documentElement.clientWidth;
					y = document.documentElement.clientHeight;
				}
				else if (document.body) // other Explorers
				{
					x = document.body.clientWidth;
					y = document.body.clientHeight;
				}
			 	
			 	document.getElementById('fig_'+id).style.left = (x/4)+'px';
			 	document.getElementById('fig_'+id).style.display = 'block';
			 	document.getElementById('fig_'+id).style.cursor = "url('../immagini/zoomout.cur'), -moz-zoom-out";
			 	document.getElementById('fig_'+id).style.position = 'absolute';
			 	document.getElementById('fig_'+id).style = 'z-index = "70"';
			 }
			
			function HideImg(id)
			 {
			 	document.getElementById('fig_'+id).style.display = 'none';
			 }
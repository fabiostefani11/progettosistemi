function favorite(addr, titolo)
				{
					if ( navigator.appName != 'Microsoft Internet Explorer' )
						{ window.sidebar.addPanel(titolo, addr,""); }
					else { window.external.AddFavorite(addr, titolo); }
				}
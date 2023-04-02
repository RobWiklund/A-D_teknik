## Installation och konfiguration av NGINX

Källa: [Youtube](https://www.youtube.com/watch?v=_zoDkXyXrx4)

### installera NGINX

`
sudo apt install nginx
`

### installera bibliotek för htpasswd
`
sudo apt install apache2-utils
`

Skapa användaren admin och ett lösenord

`
sudo htpasswd -c /etc/nginx/.htpasswd admin
`

### Auth

`
sudo nano /etc/nginx/sites-available/default
`

Denna kod konfigurerar NGINX som en omvänd proxy för WebSocket-anslutningar uppgraderar anslutningen till en WebSocket-anslutning och vidarebefordra den till lämplig slutpunkt. WebSocket används i Node-RED och utan denna uppgardering kommer Node-RED inte fungera korrekt.

I koden aktiveras också autentisering för alla webbsidor under `*/ui/`. Autentisering behövs i inte under `*/` eftersom denna sökväg är flödet eller flow för Node-RED där autentisering aktiveras i [Node-RED instruktioner](Node-RED.md#lösenordskyddad-flow)

``` nginx
 location / {
	# ------------- Proxy pass ------------- #
        proxy_pass http://localhost:1880/; # Pass everything

	# ------------- Websocket support ------------- #
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "Upgrade";
        proxy_set_header Host $host;
        }

 location /ui/ {
        # ------------- Auth ------------- #
        auth_basic "admin";
        auth_basic_user_file /etc/nginx/.htpasswd;

	# ------------- Proxy pass ------------- #
        proxy_pass http://localhost:1880/ui/;

        # ------------- Websocket support ------------- #
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "Upgrade";
        proxy_set_header Host $host;
        }
```


## Starta om NGINX
`
sudo systemctl restart nginx
`

Nginx startar automatiskt vid boot/reboot

## Åtkomst
Eftersom `Port 1880` som Node-RED använder som standard speglas över till `port 80` kommer endast `port 80` att behövas öppnas i brandväggen på det virtuella nätverket för att komma åt webbsidan.

Läs mer om nätverkssäkerhetsgrupper här: [Microsoft](https://learn.microsoft.com/en-us/azure/virtual-network/network-security-groups-overview)

Node-RED kommer att fortsätta köra programmet på `Port 1880`. Detta betyder att http://localhost:1880/ui/ förblir oskyddad på det lokala nätverket.

### För att komma åt webbsidan behövs endast IP-adressen för den virtuella maskinen slås in i en webbläsare.

Till exempel http://74.234.25.164 för att komma åt flow i Node-Red.

Eller: http://74.234.25.164/ui för att komma åt dashboard i Node-Red.

## Felsökning
Vid felet 502 Bad Gateway

![502 Bad Gateway](./fig/Error_502.png)

Detta beror inte på fel i NGINX utan detta kan betyda att Node-Red inte kör eftersom NGINX inte hittar något att visa från `Port 1880`. Starta om Node-RED, ifall fel fortfarande uppstår läs mer om fel i Node-RED [här](Node-RED.md#felsökning).
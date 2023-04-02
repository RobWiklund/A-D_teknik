# Anslut till VM och installera Node.js & Node-RED

Läs mer hur man kör Node-RED inuti Microsoft Azure: [nodered.org](https://nodered.org/docs/getting-started/azure)

### Anslut till den virtuella datorn via SSH

`
ssh [NAME]@[IP]
`

Till exempel:
`
ssh IoTDevice@76.214.21.134
`

## Installera Node-RED på den virtuella datorn

`
curl -sL https://deb.nodesource.com/setup_14.x — Node.js 14 LTS "Fermium" | sudo -E bash -
`

`
sudo apt-get install -y nodejs
`

`
sudo npm install -g --unsafe-perm node-red
`


## Kör Node-RED

Node-RED startar automatiskt med hjälp av PM2.
Läs mer här:
[nodered.org](https://nodered.org/docs/faq/starting-node-red-on-boot)
och
[pm2.keymetrics.io](https://pm2.keymetrics.io/docs/usage/process-management/)

### Autostarta Node-RED med systemet

`
sudo npm install -g --unsafe-perm pm2
`

Hitta filsökvägen för Node-RED

`
which node-red
`

Som output fås till exempel: `/usr/bin/node-red`


Lägg till filsökvägen i kommandot

`
pm2 start /usr/bin/node-red -- -v
`

`
pm2 save
`

`
pm2 startup
`


### Kontrollera om Node-Red kör på port 1880

`
sudo lsof -i:1880
`

En prompt kommer upp där ett lösenord kan skrivas in.
> **OBS! Kopiera lösenordets hash till nästa steg** 


## Lösenordskyddad flow

### Skapa lösenord

Skapa hash av ett lösenord [nodered.org](https://nodered.org/docs/user-guide/runtime/securing-node-red)

`
node-red admin hash-pw
`

### Aktivera Lösenordskyddad flow

Gå in i Node-RED mappen

`
cd .node-red/
`

Ändra inställningar i settings.js.

`
sudo nano settings.js
`

Detaljerade instruktioner finns [här](https://nodered.org/docs/user-guide/runtime/securing-node-red#editor--admin-api-security).

Avmarkera och ändra.
``` js
adminAuth: {
    type: "credentials",
    users: [

        {
            username: "devops",
            password: "[PASS_HASH]", // Password protection for flow
            permissions: "*"
        }
    ]
}
```

Samma lösenord läggs in längre ner i samma fil `settings.js`
``` js
credentialSecret: "[PASS_HASH]",
```
## Felsökning

Ifall Node-RED kör men inte fungerar korrekt kan Node-RED köras i säkert läge. Eftersom Node-RED körs i PM2 kommer den att tvinga igång Node-RED igen ifall man tvingar avslut med kill -9 [PID]. Kör istället kommandot:

``` shell
pm2 stop node-red
``` 

Kör sedan Node-RED i säkert läge genom

``` shell
node-red --safe
```

efter detta kommer Node-RED dashboard att starta. Gör de ändringar som fått Node-RED att krasha. Efteråt, starta om den virtuella datorn. PM2 kommer då starta Node-RED automatiskt.
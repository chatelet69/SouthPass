let url = "http://localhost:8081";

/*let jwt = localStorage.getItem("southpassJwt");  
if (jwt !== null && jwt !== undefined && jwt !== "null") {
    document.getElementById("loginContainer").style.display = "none";
    document.getElementById("profileContainer").style.display = "block";
}*/

console.log("coucou");

function getInfos() {
    const emailValue = document.getElementById("emailInput").value;
    document.getElementById("emailInput").value = "jean";
    console.log(emailValue);
    browser.storage.local.set("testEmail", emailValue);
    localStorage.setItem("testEmail", emailValue);
    const passValue = document.getElementById("passInput").value;
    const masterValue = document.getElementById("masterPassInput").value;
    let res = getJwt(emailValue, passValue, masterValue)
    if (res != null) alert("Connexion réussie !");
    else alert("Erreur de connexion");
}

async function getJwt(email, password, masterPass) {
    try {
        let jwt = localStorage.getItem("southpassJwt");
        if (jwt !== null && jwt !== undefined && jwt !== "null") {
            successConnect();
            return jwt;
        } else {
            const finalUrl = `${url}/login`;
            const res = await fetch(finalUrl, {
                method: "POST",
                //mode: "cors",
                headers: {
                    "Content-Type": "application/x-www-form-urlencoded"
                },
                body: `email=${email}&password=${password}&masterPassword=${masterPassword}`
            });
            const data = await res.json();
            if (data && data.jwt) {
                localStorage.setItem("southpassJwt", data.jwt);
                document.getElementById("loginContainer").style.display = "none";
                document.getElementById("profileContainer").style.display = "block";
                return data.jwt;
            }
        }
        return null;
    } catch (error) {
        return null;
    }
}

function scriptError(error) {
    console.error(
      `Erreur d'exécution du script : ${error.message}`,
    );
}

/*browser.tabs
  .executeScript({ file: "./index.js" })
  .then(getInfos)
  .catch(scriptError);*/
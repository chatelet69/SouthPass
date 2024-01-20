document.body.style.border = "5px solid red";
const baseUrl = "http://localhost:8081";
let currentWebsite = window.location.href;
let currentHostname = window.location.hostname;
let currentHost = window.location.host;
process(currentWebsite);

//window.addEventListener('load', process(currentHost));

async function getJwt(email, password, masterPass) {
    try {
        let jwt = localStorage.getItem("southpassJwt");
        console.log(typeof jwt, jwt);
        if (jwt !== null && jwt !== undefined && jwt !== "null") {
            return jwt;
        } else {
            const finalUrl = `${baseUrl}/login`;
            const res = await fetch(finalUrl, {
                method: "POST",
                //mode: "cors",
                headers: {
                    "Content-Type": "application/x-www-form-urlencoded"
                },
                body: `email=${email}&password=${password}&masterPassword=${masterPass}`
            });
            const data = await res.json();
            console.log(data);
            if (data && data.jwt) {
                localStorage.setItem("southpassJwt", data.jwt);
                return data.jwt;
            }
        }
        return null;
    } catch (error) {
        return null;
    }
}

async function process(website) {
    try {
        let jwt = await getJwt(infos.email, infos.password, infos.masterPassword);
        if (jwt != false && jwt != null) {
            let credentials = await getCredentialsOfWebsite(jwt, currentHost);
            if (credentials && credentials.id) {
                let loginInputs = document.querySelectorAll('input[type="email"], input[type="username"], input[name="username"], input[name="email"]');
                let passwordInputs = document.querySelectorAll('input[type="password"], input[name="password"]');
    
                loginInputs.forEach(loginInput => { loginInput.value = credentials.login; });
                passwordInputs.forEach(passwordInput => { passwordInput.value = credentials.password; });
            }
        }
    } catch (error) {
        throw error;
    }
}

async function getCredentialsOfWebsite(jwt, website) {
    try {
        const finalUrl = `${baseUrl}/getCredential?website=${website}`;
        const res = await fetch(finalUrl, {
            method: "GET",
            mode: "cors",
            headers: {
                "Authorization": jwt
            }
        });
        const data = await res.json();
        return data;
    } catch (error) {
        return undefined;
    }
}

/*window.addEventListener("locationchange", function(){
    console.log(window.location.href);
});*/
console.log("#page_common.js");

function getCookieValue(key) {
    const cookies = document.cookie.split('; ');
    console.log(cookies);
    for (let cookie of cookies) {
        const [name, value] = cookie.split('=');
        if (name === key) {
            return decodeURIComponent(value);
        }
    }
    return null;
}
const COOKIE_NAME = "cpp_login_1";
const main = function(){
  const value = getCookieValue(COOKIE_NAME)
  console.log("value=", value);
  if(!value){
    location.href = "/login";
  }
}
main();

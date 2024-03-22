import cgi
import cgitb
import http.cookies

# enable error reporting
cgitb.enable()

# get the form data submitted by the user
form = cgi.FieldStorage()

name = form.getvalue("name")

cookie = http.cookies.SimpleCookie()
cookie["name"] = name

print("Content-Type: text/html\r")
print("Status: 200\r")
print("\r")
ubmit" value="Submit">')
print('</form>')
print("</body></html>")

print(cookie.output())

# while True:
#     print("This loop will run indefinitely. Press Ctrl+C to exit.")
    # You can add your code here that you want to execute repeatedly
    # For example:
    # time.sleep(1)  # Uncomment this line if you want to introduce a delay

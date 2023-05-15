# from poster.encode import multipart_encode
# from poster.streaminghttp import register_openers
# import urllib2

# # Register the streaming http handlers with urllib2
# register_openers()

# # Use multipart encoding for the input files
# datagen, headers = multipart_encode({ 'files[]': open('example.bak', 'rb')})

# # Create the request object
# request = urllib2.Request('https://www.rebasedata.com/api/v1/convert', datagen, headers)

# # Do the request and get the response
# # Here the BAK file gets converted to CSV
# response = urllib2.urlopen(request)

# # Check if an error came back
# if response.info().getheader('Content-Type') == 'application/json':
#     print response.read()
#     sys.exit(1)

# # Write the response to /tmp/output.zip
# with open('/tmp/output.zip', 'wb') as local_file:
#     local_file.write(response.read())

# print 'Conversion result successfully written to /tmp/output.zip!'

import requests
import sys

path = 'CEIP Database/CEIP-March10_2017.bak'

# Use multipart encoding for the input files
files = {'files[]': open(path, 'rb')}
response = requests.post('https://www.rebasedata.com/api/v1/convert', files=files)

# Check if an error came back
if response.headers['Content-Type'] == 'application/json':
    print(response.json())
    sys.exit(1)

# Write the response to /tmp/output.zip
with open('/tmp/output.zip', 'wb') as local_file:
    local_file.write(response.content)

print('Conversion result successfully written to /tmp/output.zip!')
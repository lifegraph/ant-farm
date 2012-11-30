# build file for the ant

# set up the id
MAX_ID = 16777215 # FF FF FF
INCREMENTATION = 1
NUM_BYTES = 4

current_val = 1;
# read in the current id
with open("id.c") as f:
  data = f.read().split("\n")[4:] # skip the first 5 lines
  val = ""
  for line in reversed(data):
    if len(line) > 1:
      # parse out the hex
      val += line.split(" ")[2].replace("0x", "")#.decode("hex")
    
  current_val = int(val, 16)
  print "The current value is: ", current_val

# increment the current val
current_val += INCREMENTATION
if current_val > MAX_ID: current_val = 0 # wrap around
print "Incrementing id to: ", current_val
current_val = hex(current_val).replace("0x", "").upper()

# pad current value up to 5 bytes long
if len(current_val) < NUM_BYTES*2:
  i = NUM_BYTES*2 - len(current_val) 
  while i > 0:
    current_val = "0"+current_val
    i -= 1

# split up current val into an array
print "HEX ID: ", current_val
current_vals = [current_val[i:i+2] for i in range(0, len(current_val), 2)]

with open('id.c', 'w') as f:

  f.write("""/*
  * THIS IS AN AUTOGENERATED FILE.
  * DO NOT TOUCH.
  */

""")
  hex_id = []
  i = 1
  for val in reversed(current_vals):
    if len(val) == 1: val = "0" + val 
    f.write("#define ID_" + str(i) + " 0x"+str(val) + "\n")
    i += 1

  # pad the rest with zeros
  while i < NUM_BYTES - i:
    f.write("#define ID_" + str(i) + " 0x00\n")
    i += 1
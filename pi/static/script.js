function init() {
  poll()
  $("#joystick").bind("touchstart",touchmove)
  $("#joystick").bind("touchmove",touchmove)
  $("#joystick").bind("touchend",touchend)
}

function poll() {
  $.ajax({url: "status.json"}).done(update_status)
}

function update_status(json) {
  s = JSON.parse(json)
  $("#button0").html(s["buttons"][0] ? '1' : '0')
  $("#button1").html(s["buttons"][1] ? '1' : '0')
  $("#button2").html(s["buttons"][2] ? '1' : '0')

  $("#battery_millivolts").html(s["battery_millivolts"])

  $("#analog0").html(s["analog"][0])
  $("#analog1").html(s["analog"][1])
  $("#analog2").html(s["analog"][2])
  $("#analog3").html(s["analog"][3])
  $("#analog4").html(s["analog"][4])
  $("#analog5").html(s["analog"][5])

  setTimeout(poll, 100)
}

function touchmove(e) {
  e.preventDefault()
  touch = e.originalEvent.touches[0] || e.originalEvent.changedTouches[0];
  elm = $(this).offset();
  x = touch.pageX - elm.left;
  y = touch.pageY - elm.top;
  w = $(this).width()
  h = $(this).height()

  x = (x-w/2.0)/(w/2.0)
  y = (y-h/2.0)/(h/2.0)

  if(x < -1) x = -1
  if(x > 1) x = 1
  if(y < -1) y = -1
  if(y > 1) y = 1

  left_motor = Math.round(400*(-y+x))
  right_motor = Math.round(400*(-y-x))

  if(left_motor > 400) left_motor = 400
  if(left_motor < -400) left_motor = -400

  if(right_motor > 400) right_motor = 400
  if(right_motor < -400) right_motor = -400

  setMotors(left_motor, right_motor)
}

function touchend(e) {
  e.preventDefault()
  setMotors(0,0)
}

block_set_motors = false

function setMotors(left, right) {
  $("#joystick").html(left + " "+ right)

  if(block_set_motors) return
  block_set_motors = true

  $.ajax({url: "motors/"+left+","+right}).done(setMotorsDone)
}

function setMotorsDone() {
  block_set_motors = false
}

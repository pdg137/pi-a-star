function init() {
  poll()
}

function poll() {
  $.ajax({url: "status.json"}).done(update_status)
}

function update_status(json) {
  s = JSON.parse(json)
  $("#button0").html(s[0] ? '1' : '0')
  $("#button1").html(s[1] ? '1' : '0')
  $("#button2").html(s[2] ? '1' : '0')
  setTimeout(poll, 100)
}

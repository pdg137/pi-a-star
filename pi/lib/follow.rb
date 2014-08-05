require 'response_state'

class Follow < ResponseState::Service
  def initialize(a_star)
    @a_star = a_star
  end

  def call(&block)
    @a_star.send_follow_command
    sleep(0.1) # let it start

    until @a_star.get_report.follow_state == 0
      sleep(0.1)
    end

    report = @a_star.get_report

    context = { distance: report.distance, exits: report.exits }
    state = if report.end == 1
              :end
            else
              :intersection
            end

    yield send_state(state, '', context)
  end
end

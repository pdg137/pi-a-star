require 'response_state'

class Follow < ResponseState::Service
  def initialize(a_star)
    @a_star = a_star
  end

  def call(&block)
    @a_star.send_follow_command

    until @a_star.get_report.follow_state == 0
      sleep(0.01)
    end

    report = @a_star.get_report

    context = { distance: report.distance, exits: report.exits }
    state = if report.end == 1
              :end
            else
              :intersection
            end

    if report.button2?
      state = :button
    end

    yield send_state(state, '', context)
  end
end

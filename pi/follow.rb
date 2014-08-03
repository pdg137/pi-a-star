class Follow
  def initialize(a_star)
    @a_star = a_star
  end

  def go
    @a_star.send_follow_command
    sleep(0.1) # let it start

    until @a_star.get_report.follow_state == 0
      sleep(0.1)
    end

    report = @a_star.get_report
  end
end

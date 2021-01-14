defmodule Hammer.SysTick do
  use Task, restart: :permanent
  alias Hammer.Socket
  alias Hammer.Smoother

  def start_link(_opts \\ []) do
    Task.start_link __MODULE__, :tick, [100]
  end

  def tick(time) do
    s = Smoother.read()

    Socket.send_state s

    :timer.sleep time
    tick time
  end

end

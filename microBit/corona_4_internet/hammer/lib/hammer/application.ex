defmodule Hammer.Application do
  @moduledoc false

  use Application

  def start(_type, _args) do
    children = [
      %{
        id: Hammer.Socket,
        start: {Hammer.Socket, :start_link, []}
      },
      Hammer.UART
    ]

    opts = [strategy: :one_for_one, name: Hammer.Supervisor]
    Supervisor.start_link(children, opts)
  end
end

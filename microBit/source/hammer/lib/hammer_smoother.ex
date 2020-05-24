defmodule Hammer.Smoother do
  use GenServer
  alias Hammer.Message.State

  # Public API

  def start_link(_opts \\ []) do
    GenServer.start_link __MODULE__, 10, [name: __MODULE__]
  end

  def insert(element) do
    GenServer.call __MODULE__, {:insert, element}
  end

  def read do
    GenServer.call __MODULE__, :read
  end

  # Callbacks

  def init(size) do
    buffer = for _ <- 1..size do
      %State{a: 0, b: 0, x: 0, y: 0, z: 0}
    end

    {:ok, buffer}
  end

  def handle_call({:insert, element}, _from, [_head | tail]) do
    {:reply, :ok, tail ++ [element]}
  end

  def handle_call(:read, _from, buffer) do
    {sx, sy, sz} = Enum.reduce buffer, {0, 0, 0}, fn s, {x, y, z} ->
      {
        x + Map.get(s, :x),
        y + Map.get(s, :y),
        z + Map.get(s, :z)
      }
    end

    size = Enum.count buffer

    last = List.last buffer

    reply = %State{
      a: Map.get(last, :a),
      b: Map.get(last, :b),
      x: Kernel.trunc(sx / size),
      y: Kernel.trunc(sy / size),
      z: Kernel.trunc(sz / size)
    }

    {:reply, reply, buffer}
  end
end

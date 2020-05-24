defmodule Hammer.Socket do
  alias Phoenix.Channels.GenSocketClient
  alias Hammer.Message.State
  alias Hammer.UART

  def start_link do
    GenSocketClient.start_link(
      __MODULE__,
      GenSocketClient.Transport.WebSocketClient,
      "ws://lubemeup.sexy:80/socket/websocket",
      [],
      [name: __MODULE__]
    )
  end

  def sign_on(student_id) do
    GenSocketClient.call __MODULE__, {:sign_on, student_id}
  end

  def send_state(s) do
    GenSocketClient.call __MODULE__, {:send_state, s}
  end

  # Callbacks

  def init(url) do
    {:connect, url, [], %{joined: false, topic: ""}}
  end

  def handle_connected(transport, state) do
    {:ok, state}
  end

  def handle_disconnected(_reason, state) do
    {:connect, %{state | joined: false}}
  end

  def handle_joined(_topic, _payload, _transport, state) do
    {:ok, %{state | joined: true}}
  end

  def handle_join_error("hardware:" <> id, _payload, _transport, state) do
    IO.puts "Server denied ID #{id}, double check your C code"
    {:ok, %{state | joined: false}}
  end

  def handle_channel_closed(_topic, _payload, _transport, state) do
    {:ok, %{state | joined: false}}
  end

  def handle_message(_topic, "reset", _payload, _transport, state) do
    UART.reset()
    {:ok, state}
  end

  def handle_message(_topic, "matrix on", _payload, _transport, state) do
    UART.turn_on_matrix()
    {:ok, state}
  end

  def handle_message(_topic, "matrix off", _payload, _transport, state) do
    UART.turn_off_matrix()
    {:ok, state}
  end

  def handle_message(_topic, _not_handled, _payload_, _transport, state) do
    {:ok, state}
  end

  def handle_call({:sign_on, student_id}, _from, transport, state) do
    topic = "hardware:#{student_id}"
    GenSocketClient.join(transport, topic)
    {:reply, :ok, %{state | topic: topic}}
  end

  def handle_call({:send_state, s}, _from, transport, state) do
    payload = %{a: s.a, b: s.b, x: s.x, y: s.y, z: s.z}

    case state.joined do
      true ->
        GenSocketClient.push(transport, state.topic, "state", payload)
        {:reply, :ok, state}

      false ->
        {:reply, :reset, state}
    end
  end

end

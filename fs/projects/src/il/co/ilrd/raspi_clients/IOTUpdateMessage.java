package il.co.ilrd.raspi_clients;


public class IOTUpdateMessage implements Comparable<IOTUpdateMessage> {
	private static final String ID_VALUE_DELIMITER = "$";
	private String id;
	private String value;
	private int retransmissionCounter;
	private int sendingInterval;
	private long timeToSend;
	private long timeSent;
	
	public IOTUpdateMessage(String id, String value, int retransmissionCounter, int sendingInterval) {
		this.id = id;
		this.value = value;
		this.retransmissionCounter = retransmissionCounter;
		this.sendingInterval = sendingInterval;
		timeToSend = System.currentTimeMillis() + sendingInterval;
	}
	
	public void setTimeSent(long timeSent) {
		this.timeSent = timeSent;
	}
	
	public long getTimeSent() {
		return timeSent;
	}
	
	public long getTimeToSend() {
		return timeToSend;
	}
	
	public String getID() {
		return id;
	}
	
	public int decreaseCounter() {
		--retransmissionCounter;
		return retransmissionCounter;
	}
	
	public int getRetransmissionCounter() {
		return retransmissionCounter;
	}
	
	public int getInterval() {
		return sendingInterval;
	}
	
	public String getMessage() {
		return id + ID_VALUE_DELIMITER + value;		
	}
	
	public void updateTimeToSend() {
		timeToSend = System.currentTimeMillis() + sendingInterval;
	}

	@Override
	public int compareTo(IOTUpdateMessage msg) {
		return (int) (msg.timeToSend - this.timeToSend);
	}

	public void updateTimeToSend(long l) {
		timeToSend = System.currentTimeMillis() + l;
	}
	
}

# TCP chat
## Jak uruchomić

### Budowanie ze źródła

```bash
git clone git@github.com:jakubwiczkowski/tcp-chat.git
cd dining--chat/
mkdir build/ && cd build/
cmake ..  
```

### Uruchomienie programu

```bash
cd build

# Running the chat server 
./server/server

# Running the chat client
./client/client
```

## Opis programu

Program ma na celu umożliwienie w czasie rzeczywistym na wymianę wiadomościami tekstowymi pomiędzy wszystkimi połączonymi
do niego klientami (użytkownikami). Aby to zrealizować zaprojektowano wielowątkowego klienta oraz serwer ze wszystkimi
potrzebnymi mechanizmami synchronizacyjnymi pomiędzy wątkami. Dane wysyłane między klientem a serwerem są specjalnie ustrukturyzowane
w pakiety zawarte w submodule `protocol`. Pakiety te są enkodowane i dekodowane specjalnie przygotowanymi koderami, które
również znajdują się w submodule `protocol`.

## Wątki

### Klient

Klient czatu używa dwa wątki - jeden jest do pobierania wiadomości od użytkownika i odsyłaniu tych wiadomości do serwera,
a drugi przetwarza kolejkę pakietów do wysłania. Oba wątki działają tak długo, jak klient będzie połączony z serwerem.

### Serwer

Serwer działa na wielu wątkach. Najważniejszym jest wątek nasłuchiwania połączeń, gdyż działa on cały czas w tle
aby umożliwić nowym użytkownikom połączenie się. Każdy użytkownik po pomyślnym połączeniu dostaje też swój wątek,
który ma za zadanie przetwarzać wysyłane do socketu przez klienta dane. Dzięki przydzieleniu każdemu użytkownikowi swojego
wątku możliwe jest utrzymywanie połączeń z wieloma użytkownikami na raz.

## Sekcje krytyczne

Sekcje krytyczne zostały rozwiązane w inny sposób niż standardowo, gdyż została stworzona pomocnicza klasa `synced`. Ta 
klasa jest wrapperem na obojętnie jaką zmienną i pozwala ona na dotarcie i modyfikacje informacji w niej zawartej w
sposób bezpieczny.

1. Sekcja dodawania pakietów do kolejki wysyłania
```c++
void client::send_packet(std::unique_ptr<packet> to_send) {
    this->packet_queue.run(
        [to_send = std::move(to_send)]
    (std::queue<std::unique_ptr<packet>>& queue) mutable {
            queue.push(std::move(to_send));
        });
}
```
2. Sekcja pobierania pakietu z kolejki wysyłania
```c++
this->packet_queue.run(
    [&](std::queue<std::unique_ptr<packet>>& queue) mutable {
        to_send = std::move(queue.front());
        queue.pop();
    });
```
3. Sekcja wysyłania pakietu z wiadomością do wszystkich połączonych klientów
```c++
this->client_map.run([&client, &send_message_packet](std::unordered_map<int, chat_client>& value) {
     for (auto [_, target_client]: value) {
         std::unique_ptr<chat::clientbound::send_message> to_send =
             std::make_unique<chat::clientbound::send_message>(client.get_username(), send_message_packet.get_message());

         target_client.send_packet(std::move(to_send));
     }
});
```
4. Sekcja dodawania klienta do mapy połączonych klientów
```c++
this->client_map.run([&](std::unordered_map<int, T>& value) {
    value.emplace(connfd, client);
});
```
5. Sekcja usuwania klienta z mapy połączonych klientów
```c++
this->client_map.run([&](std::unordered_map<int, T>& value) {
    value.erase(connfd);
});
```


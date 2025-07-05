# Pokemon Oyunu
🇹🇷 Türkçe | [🇬🇧 English](README.md)

🎮 **Pokemon Oyun Simülasyonu - C++ Dönem Projesi**

## 📌 Proje Hakkında
Bu proje, C++ dilinde geliştirilen Pokémon temalı bir oyun simülasyonudur. **Nesne Yönelimli Programlama (OOP)** prensiplerini uygulamak amacıyla hazırlanmıştır.

Kullanıcılar **oyuncu** veya **admin** olarak giriş yapabilir. Oyuncular rastgele Pokémon'larla karşılaşır, savaşıp yakalayabilir veya kaçabilir. Adminler sistem verilerini güncelleyebilir (Pokémon, hareket, kullanıcı, poketopları).

## 🧪 Kullanılan OOP Kavramları
- **Kalıtım (Inheritance)**
- **Bileşim (Composition)**
- **Soyut Sınıf (Abstract Class)**
- **Modüler Programlama**

## 🕹️ Oyuncu Özellikleri
- Pokémonlarımı listele/sil
- Keşfet ve rastgele Pokémon'la karşılaş
- Saldırı yap (hareket seç)
- Poketop fırlat
- Kaç ve menüye dön
- Oturumu kapat

## 🔧 Admin Özellikleri
- Pokémon, hareket, top ve kullanıcı verilerini yönet
- Yeni veri ekle/sil
- `.txt` dosyalarına değişiklikleri kaydet

## 📄 Dosya Açıklamaları
- `pokemons.txt`: Pokémon bilgileri (İsim, HP, Atak, Defans, Hız)
- `moves.txt`: Hareket bilgileri (İsim, Güç)
- `pokeballs.txt`: Top yakalama oranları (İsim, Yakalama Oranı)
- `users.txt`: Kullanıcı adı, şifre, rol bilgileri

## ⚠️ Kurallar ve Dikkat Edilecekler
- Oyuncuların en az 1, en fazla 6 Pokémon'u olabilir
- Her Pokémon 4 rastgele hareketle gelir
- Girişte 3 Pokémon ve 3 top verilir
- Hatalı girişler kontrol altındadır
- Kod okunabilir ve açıklamalıdır

## 📈 Değerlendirme Kriterleri
- Programın eksiksiz ve hatasız çalışması
- OOP prensiplerinin doğru kullanımı

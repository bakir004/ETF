using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using Microsoft.Identity.Client;

namespace ooadepazar.Controllers;

public class OpenAIController
{
    private static readonly HttpClient _httpClient = new HttpClient();

    public async Task<string> SendMessageAsync(string prompt)
    {
        string apiKey = "";
        string systemPrompt =
            "Odgovaraj samo na bosanskom jeziku. Ti is profesionalni finansijski menadzer. Tvoj je zadatak da procijenis artikle koje ti korisnik dadne, i da kazes korisniku da li je artikal dobra ponuda. Napisi odgovor u 4 dijela: dobre stvari o artiklu, lose stvari o artiklu, kako se poredi sa slicnim artiklima na trzistu, i zakljucak gdje kazes da li se korisniku isplati kupiti dati artikal.";
        
        _httpClient.DefaultRequestHeaders.Authorization =
            new AuthenticationHeaderValue("Bearer", apiKey);

        var requestBody = new
        {
            model = "gpt-4o",
            messages = new[]
            {
                new { role = "system", content = systemPrompt },
                new { role = "user", content = prompt }
            }
        };

        var content = new StringContent(
            JsonSerializer.Serialize(requestBody),
            Encoding.UTF8,
            "application/json"
        );

        var response = await _httpClient.PostAsync("https://api.openai.com/v1/chat/completions", content);
        response.EnsureSuccessStatusCode();

        using var responseStream = await response.Content.ReadAsStreamAsync();
        var json = await JsonSerializer.DeserializeAsync<JsonElement>(responseStream);

        var reply = json
            .GetProperty("choices")[0]
            .GetProperty("message")
            .GetProperty("content")
            .GetString();

        return reply;
    }
}
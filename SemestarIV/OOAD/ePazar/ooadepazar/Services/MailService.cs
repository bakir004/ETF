using System.Net;
using System.Net.Mail;

public class MailService : IMailService
{
    private readonly IConfiguration _config;

    public MailService(IConfiguration config)
    {
        _config = config;
    }

    public async Task SendEmailAsync(string email, string content)
    {
        var smtpClient = new SmtpClient(_config["Smtp:Host"])
        {
            Port = int.Parse(_config["Smtp:Port"]),
            Credentials = new NetworkCredential(_config["Smtp:Username"], _config["Smtp:Password"]),
            EnableSsl = true
        };

        var mail = new MailMessage
        {
            From = new MailAddress(_config["Smtp:Username"]),
            Subject = "ePazar: NOTIFIKACIJA",
            Body = content,
            IsBodyHtml = false
        };

        mail.To.Add(email);
        await smtpClient.SendMailAsync(mail);
    }
}
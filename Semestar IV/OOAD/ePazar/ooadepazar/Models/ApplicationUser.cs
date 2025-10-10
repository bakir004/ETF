using System;
using System.ComponentModel.DataAnnotations;
using Microsoft.AspNetCore.Identity;

namespace ooadepazar.Models;

public class ApplicationUser : IdentityUser
{
    public string Ime { get; set; }
    public string Prezime { get; set; }
    public string Adresa { get; set; }
    public string EmailAdresa { get; set; }
    public string BrojTelefona { get; set; }
    public Uloga Uloga { get; set; } // Admin, Kurirska_Sluzba, Korisnik
    public DateTime DatumRegistracije { get; set; } = DateTime.Now;
    public string? KurirskaSluzba { get; set; }
}